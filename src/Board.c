/*  Board.c   */

#include <stdio.h>
#include "Board.h"
#include "Move.h"

void print_fun(int Board[8][8])
{
	printf("  +----+----+----+----+----+----+----+----+\n");   // border
	for(int i = 0; i < 8; i++)
	{
		//Prints peices of board in relation to peice 
		printf("%d |", (8 - i));					
		for(int j = 0; j < 8; j++)
		{
			if(Board[i][j] == bRook)						
				printf(" bR |");
			else if(Board[i][j] == bKnight)
                        	printf(" bN |");				
			else if(Board[i][j] == bBishop)
                        	printf(" bB |");
                        else if(Board[i][j] == bQueen)
                        	printf(" bQ |");
                        else if(Board[i][j] == bKing)
                        	printf(" bK |");
                        else if(Board[i][j] == bPawn)
                          	printf(" bP |");
                        else if(Board[i][j] == wRook)
                        	printf(" wR |");
                        else if(Board[i][j] == wKnight)
                        	printf(" wN |");
                        else if(Board[i][j] == wBishop)
                        	printf(" wB |");
                        else if(Board[i][j] == wQueen)
                        	printf(" wQ |");
                        else if(Board[i][j] == wKing)
                        	printf(" wK |");
			else if(Board[i][j] == wPawn)
                        	printf(" wP |");
                        else if(Board[i][j] == 0)
                        	printf("    |");
		}
                printf("\n");
                printf("  +----+----+----+----+----+----+----+----+\n");   //prints seperate lines 
	}
        printf("     a    b    c    d    e    f    g    h  \n");
}
