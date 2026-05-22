#ifndef _GAME_H
#define _GAME_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isended(int board[]);
// coordinates to number e.g. C 1 -> 22
int ctn(char coord[]);
void printboard(int board[]);
int makemove(int board[], char from[], char to[]);

#endif // _GAME_H
