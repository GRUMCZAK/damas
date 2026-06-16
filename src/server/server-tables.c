#include "server-tables.h"
#include <stddef.h>
#include <string.h>
#include <unistd.h>

Game games[MAX_GAMES] = {};
unsigned int games_len = 0;
Player players[MAX_PLAYERS] = {};
unsigned int players_len = 0;

int add_player(Player new) {
  if (players_len == MAX_PLAYERS) {
    return -1;
  }
  for (int i = 0; i < MAX_PLAYERS; i++) {
    // Technically fds are non-negative, but fd 0 is stdin 99.999% of the time
    if (players[i].clientfd == 0) {
      players[i] = new;
      players_len++;
      return 0;
    }
  }
  return -1;
}

Player *find_player(int fd) {
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (players[i].clientfd == fd) {
      return &players[i];
    }
  }

  return NULL;
}
void close_player(Player *player) {
  if (player->clientfd == 0) {
    return;
  }
  close(player->clientfd);
  memset(player, 0, sizeof *player);
  players_len--;
}
void close_all_players() {
  for (int j = 0; j < MAX_PLAYERS; j++) {
    close_player(&players[j]);
  }
}
