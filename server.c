#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MESSAGE_SIZE 256
const unsigned short PORT = 8000;
const char *LOG_PREFIX = "damas-server: ";

int sockfd = -1;
typedef struct {
  char name[32];
  int clientfd;
} Player;
typedef struct {
  Player player1;
  Player player2;
} Game;
Game games[64];
unsigned int ngames = 0;
Player players[128];
unsigned int nplayers = 0;

#define ARR_LEN(arr) sizeof arr / sizeof arr[0]

#define printerror(x) _printerror(__FILE__, __LINE__, __func__, x)
void _printerror(const char *fileName, int lineNumber, const char *funcName,
                 const char *calledFunction) {
  char buf[256];
  snprintf(buf, 256, "%s%s:%d in %s: %s -> %d", LOG_PREFIX, fileName,
           lineNumber, funcName, calledFunction, errno);
  perror(buf);
}
#define errorout(x) _errorout(__FILE__, __LINE__, __func__, #x, (x))
#define errorout_msg(x, y) _errorout(__FILE__, __LINE__, __func__, y, (x))
void _errorout(const char *fileName, int lineNumber, const char *funcName,
               const char *calledFunction, int err) {
  if (err < 0) {
    _printerror(fileName, lineNumber, funcName, calledFunction);
    if (sockfd != -1) {
      close(sockfd);
    }
    exit(1);
  }
}

int main(void) {
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
          printerror("accept()");
          continue;
        }

        event.data.fd = clientfd;
        event.events = EPOLLIN;
        errorout(epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &event));

        Player new = {
            .clientfd = clientfd,
        };
        players[nplayers++] = new;
      } else {
        for (int j = 0; j < nplayers; j++) {
          Player player = players[j];
          if (player.clientfd != event.data.fd)
            continue;

          char buffer[128];
          ssize_t io_ret = read(player.clientfd, buffer, sizeof buffer);
          buffer[io_ret] = '\0';
          // TODO: add parsing logic of client message, i.e. login in, playing
          // etc also add error handling also we need to dynamicly change number
          // of els in games/clients -> vector?
          printf("Message from client: %s\b", buffer);

          io_ret = write(player.clientfd, buffer, (size_t)io_ret);
        }
      }
    }
  }

  for (int j = 0; j < nplayers; j++) {
    close(players[j].clientfd);
  }
  close(sockfd);

  return 0;
}
