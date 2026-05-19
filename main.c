#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isended(int board[])
{
    int is1 = 0, is2 = 0;
    for(int i=0; i < 100; i++)
    {
        if(board[i] == 1)
            is1 = 1;
        if(board[i] == 2)
            is2 = 1;
    }

    if(is1 == 0 )
    {
        printf("Drużyna 2 wygrała!\n");
        return 1;
    }
    if(is2 == 0 )
    {
        printf("Drużyna 1 wygrała!\n");
        return 1;
    }
    return 0;
}

int ctn(char coord[]) // coordinates to number e.g. C 1 -> 22
{
    printf("Wybrano współrzędne: %c%c\n", coord[0], coord[1]);
    if((int)coord[0] < 65 || (int)coord[0] > 74 || (coord[1] - '0') > 9 || (coord[1] - '0') < 0)
    {
        printf("Podano nieistniejące współrzędne");
        return -1;
    }
    int number = ((int)coord[0] - 65 ) * 10;
    number += coord[1] - '0'; // we substract '0' so that we don't get an ASCII code of number, but the number itself
    //printf("number equals: %d\n", number+1); // because we index our chessboard from 0, every square number is shifted by one
    return number;
}

void printboard(int board[])
{
    // squares on the chessboard are being numbered from top-left corner starting from 0 to 99
    printf("   0 1 2 3 4 5 6 7 8 9\n");
    printf("  +-------------------+\n");
    for(int i=0; i < 100; i++)
    {
        if( i % 10 == 0)
            printf("%c ", (char)(65+i/10)); 
        switch (board[i]){
            case 0:
                printf("| ");
                break;
            case 1:
                printf("|1");
                break;
            case 2:
                printf("|2");
                break;
        }
        if(i % 10 == 9)
        {
            printf("|\n");
            printf("  +-------------------+\n"); 
        }
    }
    printf("   0 1 2 3 4 5 6 7 8 9\n\n");
}

int makemove(int board[], char from[], char to[])
{
    int f = ctn(from), t = ctn(to);
    printf("board od t: %d\n", board[t]);
    if(board[t] != 0)
    {
        printf("Na miejscu docelowym znajduje się pion - ruch niedozwolony\n");
        return -1;
    }
    if(board[f] == 0)
    {
        printf("Na miejscu źródłowym nie znajduje się pion - ruch niedozwolony\n");
        return -1;
    }
    if(abs(t/10-f/10) != 2 || abs(t%10 - f%10) !=  2)
    {
        printf("Cel i źródło nie leżą od siebie w odległości dwóch pól po skosie\n");
        return -1;
    }
    if(board[(f+t)/2] == 0) // point lying on the vertical between source and target field is the arythmetic average of their coordinates
    { // If f is even, t is even as well. If f is odd, t is odd as well - so we can peacefully divide by 2
        printf("Ruszając się w ten sposób pion nikogo nie przeskoczy - ruch niedozwolony\n");
        return -1;
    }
    if((board[(f+t)/2] + board[f]) % 2 != 0) // if the two pawns that interacted were from different teams, then the one that
        board[(f+t)/2] = 0;                  // has been jumped over has to go :(

    int temp = board[f];
    board[f] = 0;
    board[t] = temp;
}

int main()
{   //starting positions of the pawns
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
    /*printboard(board);
    char c[3]; // place for null byte is needed
    fgets(c, sizeof(c), stdin);  
    ctn(c);*/

    while (isended(board) == 0 )
    {
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