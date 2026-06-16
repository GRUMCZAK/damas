#ifndef _PLAYER_MESSAGES_H
#define _PLAYER_MESSAGES_H

#include "server-tables.h"

ServerMsg execute_player_msg(Player *player);
ServerMsg execute_player_login_msg(Player *player, ClientLoginMsg *msg);
ServerMsg execute_player_move_msg(Player *player, ClientMoveMsg *msg);

#endif // _PLAYER_MESSAGES_H
