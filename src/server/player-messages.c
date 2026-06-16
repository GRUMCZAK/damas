#include "player-messages.h"
#include <string.h>

ServerMsg execute_player_msg(Player *player) {
  ClientMsg msg = {
      .tag = player->current_msg_tag,
  };

  // .login is part of union so every message type starts at the same address
  memcpy(&msg.login, player->read_buffer,
         ClientMsgLengths[player->current_msg_tag]);

  switch (msg.tag) {
  case CLIENT_MSG_LOGIN:
    return execute_player_login_msg(player, &msg.login);
  case CLIENT_MSG_MOVE:
    return execute_player_move_msg(player, &msg.move);
  }
}

ServerMsg execute_player_login_msg(Player *player, ClientLoginMsg *msg) {
  // TODO: validate player login and password
  ServerLoginResponse res = {.ok = 1};
  return (ServerMsg){.tag = SERVER_LOGIN_RESPONSE, .login_res = res};
}
ServerMsg execute_player_move_msg(Player *player, ClientMoveMsg *msg) {}
