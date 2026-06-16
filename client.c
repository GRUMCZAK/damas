#include "src/client/game.h"
#include <stdio.h>

#define _CLIENT

int main() {
  // starting positions of the pawns
  // clang-format off
    int board[100] = {
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
        0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 0, 2, 0, 2, 0, 2, 0, 2,
        2, 0, 2, 0, 2, 0, 2, 0, 2, 0,
        0, 2, 0, 2, 0, 2, 0, 2, 0, 2,
        2, 0, 2, 0, 2, 0, 2, 0, 2, 0
    };
  // clang-format on

  while (isended(board) == 0) {
    printboard(board);
    char from[4], to[4]; // place for null byte is needed
    printf("Podaj pole Z którego chcesz ruszyć piona: ");
    fgets(from, sizeof(from), stdin);
    printf("Podaj pole NA którego chcesz ruszyć piona: ");
    fgets(to, sizeof(to), stdin);
    printf("\n");
    makemove(board, from, to);
  }
}
