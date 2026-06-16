#include "src/common/helpers.h"
#include "src/common/logs.h"
#include "src/common/transport.h"
#include "src/server/player-messages.h"
#include "src/server/server-tables.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define _SERVER

#define ARR_LEN(arr) sizeof arr / sizeof arr[0]
const unsigned short PORT = 8000;

int sockfd = -1;

int main() {
  signal(SIGPIPE, SIG_IGN);

  errorout(sockfd = socket(AF_INET, SOCK_STREAM, 0));
  printf("sockfd=%d\n", sockfd);
  errorout(
      setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)));

  struct sockaddr_in s_sockaddr_in = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr("127.0.0.1"),
      .sin_port = htons(PORT),
  };

  errorout(
      bind(sockfd, (struct sockaddr *)&s_sockaddr_in, sizeof s_sockaddr_in));

  errorout(listen(sockfd, 5));

  int epollfd;
  errorout(epollfd = epoll_create1(0));
  struct epoll_event s_event = {
      .data.fd = sockfd,
      .events = EPOLLIN,
  };
  errorout(epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &s_event));

  struct epoll_event events[128];
  while (1) {
    int nevents;
    errorout(nevents = epoll_wait(epollfd, events, ARR_LEN(events), -1));
    for (int i = 0; i < nevents; i++) {
      struct epoll_event event = events[i];
      if (event.data.fd == sockfd) {
        struct sockaddr_in client_addr;
        int clientfd = accept(sockfd, (struct sockaddr *)&client_addr,
                              &(socklen_t){sizeof client_addr});
        if (clientfd == -1) {
          printerror("accept(...)");
          continue;
        }

        event.data.fd = clientfd;
        event.events = EPOLLIN;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event) == -1) {
          printerror("epoll_ctl(, EPOLL_CTL_ADD, ...)");
          close(clientfd);
          continue;
        }

        Player new = {
            .clientfd = clientfd,
            .read_state = READING_TAG,
            .current_msg_length = TAG_LENGTH,
        };
        if (add_player(new) == -1) {
          close(clientfd);
        }
      } else {
        Player *player = find_player(event.data.fd);
        if (player == NULL) {
          close(event.data.fd);
          continue;
        }

        ssize_t io_ret = read(
            player->clientfd, player->read_buffer + player->read_buffer_length,
            player->current_msg_length - player->read_buffer_length);
        if (io_ret <= 0) {
          if (io_ret < 0) {
            printerror("player read(...)");
          }
          close_player(player);
          continue;
        }
        player->read_buffer_length += io_ret;

        if (player->read_buffer_length < player->current_msg_length) {
          continue;
        }

        if (player->read_state == READING_TAG) {
          player->current_msg_tag = player->read_buffer[0];
          player->read_state = READING_LENGTH;
          player->current_msg_length = LENGTH_LENGTH;
          player->read_buffer_length = 0;
        } else if (player->read_state == READING_LENGTH) {
          memcpy(&player->current_msg_length, player->read_buffer,
                 sizeof player->current_msg_length);
          player->read_state = READING_VALUE;
          player->read_buffer_length = 0;
        } else if (player->read_state == READING_VALUE) {
          ServerMsg msg = execute_player_msg(player);
          write(player->clientfd, &msg.login_res, ServerMsgLengths[msg.tag]);
          player->read_state = READING_TAG;
          player->current_msg_length = TAG_LENGTH;
          player->read_buffer_length = 0;
        }
      }
    }
  }

  close_all_players();
  close(sockfd);

  return 0;
}
