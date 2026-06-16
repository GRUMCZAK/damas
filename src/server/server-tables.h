#ifndef _SERVER_TABLES_H
#define _SERVER_TABLES_H

#include "../common/transport.h"
#include <stdint.h>

#define MAX_GAMES 64
#define MAX_PLAYERS 128
#define MAX_MSG_LENGTH (1 << LENGTH_LENGTH) - 1 + TAG_LENGTH + LENGTH_LENGTH

typedef struct Player {
  int clientfd; // 0 -> empty struct
  uint8_t read_buffer[MAX_MSG_LENGTH];
  uint16_t read_buffer_length;
  enum ReadState read_state;
  uint8_t current_msg_tag;
  uint16_t current_msg_length;
  char name[32];
} Player;

typedef struct Game {
  Player player1;
  Player player2;
} Game;

int add_game(Game game);
int remove_game(Game game);
int find_game(Game game);

int add_player(Player player);
Player *find_player(int fd);
int remove_player(Player *player);
void close_player(Player *player);
void close_all_players();

#endif // _SERVER_TABLES_H
