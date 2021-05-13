/*  AI.c   */

#include "AI.h"
#include "Move.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char globalValidAIMove[5];

struct coordinate
{
    int row;
    char col;
};

int AI(int board[8][8], int Color, MLIST *MoveList)
{ //for easy ai
    // iterate through the entire board and get the coordinates for all your pieces
    // use an array to store what these coordinates are, it'll be size 16 since each player has 16 pieces at the start
    struct coordinate init_positions[16];
    int index = 0; // this is to keep track of the index in the pieces array as you add to it
    for (int i = 0; i < 8; i++) // iterates to get every position of all a color's pieces
    {
        for (int j = 0; j < 8; j++)
        {
            // Building array of starting positions
            if (board[i][j] >= bPawn && board[i][j] <= bKing && Color == 1) 
            { // if color is black and Color is 2 then its black
                init_positions[index].row = '8' - i;
                init_positions[index].col = 'h' - j;
                index++;
            }
            else if (board[i][j] <= wKing && board[i][j] >= wPawn && Color == 2)
            { // if color is white and Color is 1 then its white
                init_positions[index].row = '8' - i;
                init_positions[index].col = 'h' - j;
                index++;
            }
        }
    }

    int random_number; // random piece based on 16 pieces on each side
    char move_to_make[5];
    char OrColor;
	char DesColor;
    int hasKingMoved = 0;
    int AIMove = 0;
    // build the starting move string with the initial position first like a1
    char cols[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',}; //for building random moves
    char rows[8] = {'0', '1', '2', '3', '4', '5', '6', '7',};
    int Identity, desIdentity;

    do{
        srand(time(NULL));
        random_number = rand() % index;
        move_to_make[0] = init_positions[random_number].col;
        move_to_make[1] = init_positions[random_number].row;
        OrColor = '0';
		DesColor = '0';
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                move_to_make[2] = cols[j];        //second move a3
                move_to_make[3] = rows[i] + 1;   //second move a3
                move_to_make[4] = '\0';         // needed to implement a null
                Identity = IdentityDetect(board, move_to_make, 1);
                desIdentity = IdentityDetect(board, move_to_make, 2);
                if(Identity >= wPawn && Identity <= wKing)
				    OrColor = 'w';
				else if(Identity >= bPawn && Identity <= bKing)
				    OrColor = 'b';
                else if(Identity == 0)
		        {
			        OrColor = '0';
			        continue;
		        }

                if(desIdentity >= wPawn && desIdentity <= wKing)
                    DesColor = 'w';
                else if(desIdentity >= bPawn && desIdentity <= bKing)
                    DesColor = 'b';
                else if(desIdentity == 0)
                    DesColor = '0';

                if (OrColor == DesColor)
                    continue;
                else 
                {
                    if (Identity == bPawn || Identity == wPawn)// if it's a pawn
                        AIMove = PawnMove(board, move_to_make, MoveList, 2); //check that AI move is valid or choose second valid option
                    else if (Identity == bKnight || Identity == wKnight)// if it's a knight
                        AIMove = KnightMove(board, move_to_make);
                    else if (Identity == bBishop || Identity == wBishop)// if it's a bishop
                        AIMove = BishopMove(board, move_to_make);
                    else if (Identity == bRook || Identity == wRook)// if it's a rook
                        AIMove = RookMove(board, move_to_make);
                    else if (Identity == bQueen || Identity == wQueen)// if it's a queen
                        AIMove = QueenMove(board, move_to_make);
                    else if (Identity == bKing || Identity == wKing) // if it's a king
                    {
                        AIMove = KingMove(board, move_to_make, hasKingMoved);
                        hasKingMoved++;
                    }

                    if (AIMove > 0)
                    {
		                MOVE *move = NewMove(Identity, desIdentity, move_to_make);
                        AppendMove(MoveList, move);
                        if ((canCaptureKing(board, 0) == 1 && Color % 2 == 0) || (canCaptureKing(board, 0) == 2 && Color % 2 == 1)) {
					        Takeback(board, MoveList);
					        AIMove = 0;
				        }
                    }
                }
            }  
        }
    }while (AIMove <= 0);
    
    // Failsafe to make any valid move in case no move is made by the time the do while loop concludes
    if (AIMove == 0 && IdentityDetect(board, globalValidAIMove, 1) != wKing && IdentityDetect(board, globalValidAIMove, 1) != bKing)
        AIMove = pieceMove(board, globalValidAIMove, hasKingMoved, MoveList, 2);
    else if (AIMove == 0 && (IdentityDetect(board, globalValidAIMove, 1) == wKing || IdentityDetect(board, globalValidAIMove, 1) == bKing))
        AIMove = pieceMove(board, globalValidAIMove, hasKingMoved++, MoveList, 2);

    int win = winCon(board, MoveList, Color);
    if(win == 1) // if/else for checks
    {
        printf("\nCheck for white!\n");
        return 1;
    }
    else if(win == 2)
    {
        printf("\nCheck for black!\n");
        return 2;
    }
    return 0;
}
