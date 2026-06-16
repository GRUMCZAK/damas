#ifndef _TRANSPORT_H
#define _TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

#define TAG_LENGTH 1
#define LENGTH_LENGTH 2

enum ReadState {
  READING_TAG,
  READING_LENGTH,
  READING_VALUE,
};

#pragma pack(push, 1)

typedef struct {
  uint8_t login[32];
  uint8_t password[32];
} ClientLoginMsg;
typedef struct {
  unsigned char move[2];
} ClientMoveMsg;

typedef struct {
  uint8_t ok;
} ServerLoginResponse;

#pragma pack(pop)

enum ServerMsgTag : uint8_t { SERVER_LOGIN_RESPONSE };
const size_t ServerMsgLengths[] = {
    [SERVER_LOGIN_RESPONSE] = sizeof(ServerLoginResponse),
};
typedef struct {
  enum ServerMsgTag tag;
  union {
    ServerLoginResponse login_res;
  };
} ServerMsg;

enum ClientMsgTag : uint8_t { CLIENT_MSG_LOGIN, CLIENT_MSG_MOVE };
const size_t ClientMsgLengths[] = {
    [CLIENT_MSG_LOGIN] = sizeof(ClientLoginMsg),
    [CLIENT_MSG_MOVE] = sizeof(ClientMoveMsg),
};
typedef struct {
  enum ClientMsgTag tag;
  union {
    ClientLoginMsg login;
    ClientMoveMsg move;
  };
} ClientMsg;

#endif // _TRANSPORT_H
