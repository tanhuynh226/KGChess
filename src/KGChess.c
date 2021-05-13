/*	KGChess.c	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AI.h"
#include "Board.h"
#include "Move.h"
#include "MoveLog.h"

//User Input
void My_User_Input_Game(int count, char uInpt[5]){
	if(count % 2 == 0)
		printf("\n(White's Turn) Please enter your move: ");
	else
		printf("\n(Black's Turn) Please enter your move: ");
	scanf("%s", uInpt);
}
// Function that sets conditions for types of wins 
void printWin(int win){
	if(win == 1)
		printf("\nCheck for white!\n\n");
	else if(win == 2)
		printf("\nCheck for black!\n\n");
	else if(win == 3)
		printf("\nCheckmate! White wins!\n\n");
	else if(win == 4)
		printf("\nCheckmate! Black wins!\n\n");
	else if (win == 5)
		printf("\nStalemate!\n\n");
}

int Exit_Undo(char uInpt[5],int count, int Board[8][8], MLIST *Move_List){
	char exitInpt[5];							// Char input of user 
	char undoInpt[5];							// char input of user 
	strcpy(exitInpt, "exit");
	strcpy(undoInpt, "undo");

	if (strcmp(uInpt, exitInpt) == 0){				//if user input == "exit" exits game
		printf("Exiting KGChess...\n");
		DeleteMoveList(Move_List);
		exit(0);
	}
	else if (count != 0 && strcmp(uInpt, undoInpt) == 0){       //if input is valid performs take back move 
		printf("Performing takeback move...\n");
		Takeback(Board, Move_List);
		return -1;
	} else
		return IdentityDetect(Board, uInpt, 1);
}

//Human V. Human function
MLIST *Human_Vs_Human(int Board[8][8]){
	char* OrColor;
	char* DesColor;
	int win = 0, check, hasWKingmoved = 0, hasBKingmoved = 0, desIdentity = 0, Identity = 0;
	int count = 0;
	int result = 0;
	char uInpt[5];
	MLIST *MoveList;
	MOVE *move;

	MoveList = NewMoveList();

	while(win != 3 || win != 4 || win != 5){
		do{
			print_fun(Board);
			check = 1;

			OrColor = "0";
			DesColor = "0";
			// User Inputs
			My_User_Input_Game(count, uInpt);
			Identity = Exit_Undo(uInpt, count, Board, MoveList);
			if(Identity == -1){
				count--;
				//check = 0;
				continue;
			}
			pieceColor(Identity, &OrColor);
			while((((count % 2 == 1) && *OrColor == 'w')||((count % 2 == 0) && *OrColor == 'b')) && check){
				check = 1;
				printf("\nThat's not your piece! Please try again.\n");
				print_fun(Board);
				My_User_Input_Game(count, uInpt);
				Identity = Exit_Undo(uInpt, count, Board, MoveList);
				if(Identity == -1){
					count--;
					check = 0;
				}
				pieceColor(Identity, &OrColor);
			};

			printf("Your move is: %s\n", uInpt);

			// Sets OrColor to the OrIdentity's color
			pieceColor(Identity, &OrColor);
			while(*OrColor == '0'&& check){
				check = 1;
				printf("Position %c%c is empty. Try again.\n", uInpt[0], uInpt[1]);
				print_fun(Board);
				My_User_Input_Game(count, uInpt);
				Identity = Exit_Undo(uInpt, count, Board, MoveList);
				if(Identity == -1){ 
					count--;
					check = 0;
				}
				pieceColor(Identity, &OrColor);
			}

			// Sets DesColor based on desIdentity's #
			desIdentity = IdentityDetect(Board, uInpt, 2);
			pieceColor(desIdentity, &DesColor);
			if(*OrColor == *DesColor){
				printf("Illegal move. The piece at %c%c has same %s color.\n Try again.\n", uInpt[2], uInpt[3], (*OrColor == 'w') ? "white" : "black");
				continue;
			} else {
				//Moving the piece and checking for special pieces
				if(Identity == wKing){
					result = pieceMove(Board, uInpt, hasWKingmoved++, MoveList, 1);
				} else if(Identity == bKing) {
					result = pieceMove(Board, uInpt, hasBKingmoved++, MoveList, 1);
				} else {
					result = pieceMove(Board, uInpt, 0, MoveList, 1);
				}
				//Error handling and check for illegal move where user to puts itself in check
				if(result == -1) {
					printf("Error: obstacle in path from Origin to Dest. Try again.\n");
				} else if(result == 0) {
					printf("Error: This move is not belong to this piece. Try again.\n");
				}
				else if ((canCaptureKing(Board, 0) == 1 && count % 2 == 0) || (canCaptureKing(Board, 0) == 2 && count % 2 == 1)) {
					move = NewMove(Identity, desIdentity, uInpt);
					AppendMove(MoveList, move);
					Takeback(Board, MoveList);
					result = 0;
					printf("Error: Moving this piece would put itself in check. Try again.\n");
				}
			}
		} while((*OrColor == *DesColor) || (result <= 0) || !check);
		move = NewMove(Identity, desIdentity, uInpt);
		AppendMove(MoveList, move);
		
		// Check win/tie condition of the board
		if (MoveList->Length > 2) {
			win = winCon(Board, MoveList, count);
			printf("CheckStatus: %d\n", win);
		}
		
		printWin(win);
		if(win == 3 || win == 4 || win == 5)
			break;
		count++;
	}
	print_fun(Board);
	Print2File(MoveList, win);
	return(MoveList);	
}

MLIST *Human_Vs_AI(int Board[8][8], int color){
	int win = 0, check, count = 0, hasWKingmoved = 0, hasBKingmoved = 0, desIdentity = 0, Identity = 0;
	int result = 0;
	char *OrColor;
	char *DesColor;
	char uInpt[5];
	
	MLIST *MoveList;
	MOVE *move;
	MoveList = NewMoveList();
	
	while(win != 3 || win != 4 || win != 5){
		do{
			//Initialize the beginning for first turn when Player is Black
			if(color == 2 && count % 2 == 0){
				win = AI(Board, color, MoveList);
				count++;
			}
			check = 1;
			print_fun(Board);
			
			OrColor = "0";
			DesColor = "0";
			
			// User Inputs
			My_User_Input_Game(count, uInpt);
			for (int i = 0; i < 2; i++){
				if(uInpt[0] == 'u' && count == 0){
					printf("\nInvalid use of \"undo\" (Not enough turns done)\n");
				break;
				}
				Identity = Exit_Undo(uInpt, count, Board, MoveList);
			}
			if(Identity == -1){
				count -= 2;
				continue;
			}
			pieceColor(Identity, &OrColor);
			while(count % 2 == (color - 1) && *OrColor == ('b' + 21 * (color - 1))){
				printf("\nThat's not your piece! Please try again.\n");
				print_fun(Board);
				My_User_Input_Game(count, uInpt);
			}
			printf("Your move is: %s\n", uInpt);
	
			Identity = IdentityDetect(Board, uInpt, 1);
			// piece at origin is white
			pieceColor(Identity, &OrColor);
			while(*OrColor == '0' && check){
				check = 1;
				printf("Position %c%c is empty. Try again.\n", uInpt[0], uInpt[1]);
				print_fun(Board);
				My_User_Input_Game(count, uInpt);
				for(int i = 0; i < 2; i++ ){
					Identity = Exit_Undo(uInpt, count, Board, MoveList);
				}
				if(Identity == -1){
					count -= 2;
					check = 0;
					continue;
				}
				pieceColor(Identity, &OrColor);
			}

			// piece at destination is white
			desIdentity = IdentityDetect(Board, uInpt, 2);
			pieceColor(desIdentity, &DesColor);
			if(*OrColor == *DesColor)
				{
			printf("Illegal move. The piece at %c%c has same %s color.\n Try again.\n", uInpt[2], uInpt[3], (*OrColor == 'w') ? "white" : "black");
			continue;
				}
			else
				{
				if(Identity == wKing)
				{
					result = pieceMove(Board, uInpt, hasWKingmoved++, MoveList, 1);
				} else if (Identity == bKing){
					result = pieceMove(Board, uInpt, hasBKingmoved++, MoveList, 1);
				} else   result = pieceMove(Board, uInpt, 0, MoveList, 1);

				if(result == -1)
					printf("Error: obstacle in path from Origin to Dest. Try again.\n");
				else if(result == 0)
					printf("Error: This move is not belong to this piece. Try again.\n");
				else if ((canCaptureKing(Board, 0) == 1 && count % 2 == 0) || (canCaptureKing(Board, 0) == 2 && count % 2 == 1)) {
					move = NewMove(Identity, desIdentity, uInpt);
					AppendMove(MoveList, move);
					Takeback(Board, MoveList);
					result = 0;
					printf("Error: Moving this piece would put itself in check. Try again.\n");
				}
			}
		} while((*OrColor == *DesColor) || (result <= 0));
	
		move = NewMove(Identity, desIdentity, uInpt);
		AppendMove(MoveList, move);
		win = winCon(Board, MoveList, count);
		if(win == 1)
			printf("\nCheck for white!\n");
		else if(win == 2)
			printf("\nCheck for black!\n");
		else if(win == 3 || win == 4 || win == 5)
			break;

		win = AI(Board, color, MoveList);
		
		if (color == 1) // Offset for if player is white
			count++;
	}
	print_fun(Board);
	if(win == 3)
		printf("\nCheckmate! White wins!\n\n");
	else if(win == 4)
		printf("\nCheckmate! Black wins!\n\n");
	else if (win == 5)
		printf("\nStalemate!\n\n");
	Print2File(MoveList, win);
	return(MoveList);
}

MLIST *AI_Vs_AI(int Board[8][8]){
	int win = 0, count = 0;
	
	MLIST *MoveList;
	MoveList = NewMoveList();
	while(win != 3 || win != 4 || win != 5){
		print_fun(Board);

		printf("Press enter to continue...\n");
		getchar();

		if(count % 2 == 0){
			win = AI(Board, 2, MoveList);
		}
		else if (count % 2 == 1){
			win = AI(Board, 1, MoveList);
		}

		count++;
		win = winCon(Board, MoveList, count);
		if(win == 1)
			printf("\nCheck for white!\n");
		else if(win == 2)
			printf("\nCheck for black!\n");
	}
	print_fun(Board);
	if(win == 3)
		printf("\nCheckmate! White wins!\n\n");
	else if(win == 4)
		printf("\nCheckmate! Black wins!\n\n");
	else if (win == 5)
		printf("\nStalemate!\n\n");
	Print2File(MoveList, win);
	return(MoveList);
}

// Main Program for KGChess
int main()
{
	int option = 0;
	printf("Welcome to KGChess!\n");
	int done=0;
	char fname[20];
	char ftype[] = ".txt";

	while(done==0)
	{
		// initialize chess board
		int Board[8][8]={
			{10,  8,  9, 11, 12,  9,  8, 10},
			{ 7,  7,  7,  7,  7,  7,  7,  7},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 1,  1,  1,  1,  1,  1,  1,  1},
			{ 4,  2,  3,  5,  6,  3,  2,  4}
			};
		
		printf("Please make a selection\n");
		printf("1. Human vs. Human\n");
		printf("2. Human vs. AI\n");
		printf("3. AI vs. AI\n");
		printf("4. View most recent game's move log\n");
		printf("5. Exit Game\n");
		printf("Choose option: ");
		scanf("%d", &option);
		
		switch(option)
		{
			case 1:
				Human_Vs_Human(Board);
				break;
			case 2: //human v ai
				while(1)
				{
					int color;
					printf("Please choose a color for human player.\n");
					printf("1. White\n");
					printf("2. Black\n");
					printf("3. Back to main menu\n");
					printf("Choose option: ");
					scanf("%d",&color);
					getchar();
					while(color < 1 || color > 3){
						printf("Invalid input. Please try again...\n");
						printf("Choose option: ");
						scanf("%d",&color);
					}
					if (color == 3) {
                		printf("Going back to main menu...\n");
						break;
					}
					else if (color == 1 || color == 2) {
						Human_Vs_AI(Board, color);
						break;
					}
				}
                break;
			case 3:
				AI_Vs_AI(Board);
				break;
			case 4:
			    printf("Enter the file name: ");
				scanf("%s", fname);
				strcat(fname, ftype);
				MoveLog(fname);
				break;
			case 5:
				printf("Thanks for playing. Exiting game...\n");
				done=1;
				break;
			default:
				printf("Invalid input. Please try again...\n");
				break;
		}
	}
	return 0;
}
