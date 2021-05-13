/*  Move.c   */
#include <stdio.h>
#include <stdlib.h>
/* Move.c */

#include <string.h>
#include "Move.h"
#include "Board.h"

extern char globalValidAIMove[5];

int IdentityDetect(int Board[8][8], char Move[5], int specifier){
    int File, Rank, pieceID = 0;
    if (specifier == 1) {
        File = Move[0] - 'a';
        Rank = 7 - (Move[1] - '1');
        pieceID = Board[Rank][File];
        return pieceID;
    }
    else if (specifier == 2) {
        File = (Move[2] - 'a');
        Rank = (7 - (Move[3] - '1'));
        pieceID = Board[Rank][File];
        return pieceID;
    }
    else
      return 0;
}

//Piece Color
void pieceColor(int Identity, char **Color){
	if(Identity >= wPawn && Identity <= wKing){
		*Color = "w";
	} else if(Identity >= bPawn && Identity <= bKing){
		*Color = "b";
	}else{
		*Color = "0";
	}
}

int KnightMove(int Board[8][8], char Move[5]){
	int OG_Rank, OG_File, newFile, newRank, diffRank, diffFile; 
	OG_File = Move[0] - 'a';
	OG_Rank = 7 - (Move[1] - '1');
	newFile = Move[2] - 'a';
	newRank = 7 - (Move[3] - '1');
	diffRank = abs(OG_Rank - newRank);
	diffFile = abs(OG_File - newFile);

	if(((diffRank == 2) && (diffFile == 1)) || ((diffFile == 2) && (diffRank == 1))){
		Board[newRank][newFile] = Board[OG_Rank][OG_File];
		Board[OG_Rank][OG_File] = 0;
		return 1;
	}
	else
		return 0;
}

int BishopMove(int Board[8][8], char Move[5]){
  int Rank, File, newRank , newFile , diffRank, diffFile;  //Rank = Row, File = Column 
  Rank = 7 - (Move[1] - '1'); // Mapping Row character into a index value 
  File = Move[0] - 'a';  // Mapping Column character into a index value  
  newRank = 7 - (Move[3] - '1'); // Mapping final position
  newFile = Move[2] - 'a'; //Mapping final position
  diffRank = abs(newRank - Rank); //difference between the intial and final position (Vertical/ Horizontal)
  diffFile = abs(newFile - File);//difference between the inital and final position (horizontal/Vertical)
  int x = 0;
  int y = 0;

  if(diffRank == diffFile){ // checks that change and column are same value: example 2 up and 2 left
    // IV-quadrant diagonal
    if((newRank > Rank) && (newFile > File))
      {
	// Move (x, y) to the point right after the origin in the diagonal
	x = File + 1;
	y = Rank + 1;

	// Start Looping from the next point right after the origin to the destination
	while((x < newFile) && (y < newRank))
	  {
	    // Check whether there is obstacle or not
	    if(Board[y][x] != 0)
	      break;
	    x++;
	    y++;
	  }
      }
    // III-quadrant diagonal
    else if((newRank > Rank) && (newFile < File))
      {
	// Move (x, y) to the point right after the origin in the diagonal
	x = File - 1;
	y = Rank + 1;

	// Start Looping from the next point right after the origin to the destination
	while((x > newFile) && (y < newRank))
	  {
	    if(Board[y][x] != 0)
	      break;
	    x--;
	    y++;
	  }
      }
    // I-quadrant diagonal
    else if((newRank < Rank) && (newFile > File))
      {
	// Move (x, y) to the point right after the origin in the diagonal
	x = File + 1;
	y = Rank - 1;

	// Start Looping from the next point right after the origin to the destination
	while((x < newFile) && (y > newRank))
	  {
	    if(Board[y][x] != 0)
	      break;
	    x++;
	    y--;
	  }
      }
    // II-quadrant diagonal
    else if((newRank < Rank) && (newFile < File))
      {
	// Move (x, y) to the point right after the origin in the diagonal
	x = File - 1;
	y = Rank -  1;

	// Start Looping from the next point right after the origin to the destination
	while((x > newFile) && (y > newRank))
	  {
	    if(Board[y][x] != 0)
	      break;
	    x--;
	    y--;
	  }
      }
    
    // there is obstacle in the path
    // (x, y) point cannot reach the destination
    if((x != newFile) && (y != newRank))
      {
	return -1;  // error message code - obstacle in path from Origin to Dest
      }
    // the path is clean
    else
      {
	Board[newRank][newFile] = Board[Rank][File];
	Board[Rank][File] = 0;
	return 1; // success message code. Move is finished, Board is modified
      }
  }
  else
	return 0; // error message code - this move is not belonging to Bishop
}

int PawnMove(int Board[8][8], char Move[5], MLIST *MoveList, int promotionAllowed){
	int Rank, File, newRank , newFile , diffRank, diffFile, prevDiffRank = 0;  //Rank = Row, File = Column
	char prevID[5];
	Rank = 7 - (Move[1] - '1'); // Mapping Row character into a index value 
	File = Move[0] - 'a';       // Mapping Column character into a index value  
	newRank = 7 - (Move[3] - '1'); // Mapping final position
	newFile = Move[2] - 'a';
	diffRank = abs(Rank - newRank); //difference between the intial and final position (Rank)
	diffFile = abs(File - newFile);//difference between the inital and final position (File)
	char tmp[5];
	
	// Initialize temp
	for(int i = 0; i < 5; i++) {
	  tmp[i] = Move[i];
	}
	
	// Get previous piece & move (used in en passant)
	if (MoveList->Length > 1) {
		char *prevMove = MoveList->Last->Move->NewMove;
		prevDiffRank = abs((7 - (prevMove[1] - '1')) - (7 - (prevMove[3] - '1')));
		prevID[0] = prevMove[0];
		prevID[1] = prevMove[1];
	}

	int Color = IdentityDetect(Board, Move, 1);
	int desColor = IdentityDetect(Board, Move, 2);
	if(Color == 1 && diffRank == 1 && newRank < Rank && diffFile == 0 && desColor == 0) { //Checks that color is black and new row is smaller then current row to go forward
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 0 && promotionAllowed == 1)
			Promotion(Board, Color, newRank, newFile, 0);
		else if (newRank == 0 && promotionAllowed == 2)
			Promotion(Board, Color, newRank, newFile, 1);
		return 1;
	}
	if(Color == 7 && diffRank == 1 && newRank > Rank && diffFile == 0 && desColor == 0) { //Checks that color is white and new row is larger then current row to go forward
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 7 && promotionAllowed == 1)
			Promotion(Board, Color, newRank, newFile, 0);
		else if (newRank == 0 && promotionAllowed == 2)
			Promotion(Board, Color, newRank, newFile, 1);
		return 1;
	}
	if(Color == 1 && diffRank == 2 && newRank < Rank && diffFile == 0 && desColor == 0 && Rank == 6) { //White pawn can move 2 squares forward from its initial position
		Move[3] -= 1;
		if (IdentityDetect(Board, Move, 2) == 0){ //Do we need this if statement since it is being checked in the previous if statement?
			Move[3] += 1;
			Board[newRank][newFile] = Board[Rank][File];
			Board[Rank][File] = 0;
			return 1;
		}
		else
			return -1;
	}
	if(Color == 7 && diffRank == 2 && newRank > Rank && diffFile == 0 && desColor == 0 && Rank == 1) { //Black pawn can move 2 squares forward from its initial position
		Move[3] += 1;
		if (IdentityDetect(Board, Move, 2) == 0){
			Move[3] -= 1;
			Board[newRank][newFile] = Board[Rank][File];
			Board[Rank][File] = 0;
			return 1;
		}
		else
			return -1;
	}
	if(Color == 1 && diffRank == 1 && diffFile == 1 && newRank < Rank && desColor >= bPawn){ //checks that change and column are same value (kill move)
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 0 && promotionAllowed == 1)
			Promotion(Board, Color, newRank, newFile, 0);
		else if (newRank == 0 && promotionAllowed == 2)
			Promotion(Board, Color, newRank, newFile, 1);
		return 1;
	}
	if(Color == 7 && diffRank == 1 && diffFile == 1 && newRank > Rank && desColor < bPawn && desColor != 0){ //checks that change and column are same value (kill move)
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 7 && promotionAllowed == 1)
			Promotion(Board, Color, newRank, newFile, 0);
		else if (newRank == 0 && promotionAllowed == 2)
			Promotion(Board, Color, newRank, newFile, 1);
		return 1;
	}
	tmp[3] -= 1;
	prevID[1] -= 2;
	if(Color == 1 && Rank == 3 && diffFile == 1 && newRank < Rank && newRank == 2 && IdentityDetect(Board, tmp, 2) == bPawn && IdentityDetect(Board, prevID, 1) == bPawn && prevDiffRank == 2){ //En passant for white
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		Board[newRank+1][newFile] = 0;
		return 1;
	}
	tmp[3] += 2;
	prevID[1] += 4;
	if(Color == 7 && Rank == 4 && diffFile == 1 && newRank > Rank && newRank == 5 && IdentityDetect(Board, tmp, 2) == wPawn && IdentityDetect(Board, prevID, 1) == wPawn && prevDiffRank == 2){ //En Passant for black
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		Board[newRank-1][newFile] = 0;
		return 1;
	}
	return 0;
}

void Promotion(int Board[8][8], int Color, int Rank, int File, int AI){
	int promotion = 1, done = 0;
	if (AI == 0) {
		printf("\nWhat piece would you like to promote to?\n");
		printf("1. Queen\n");
		printf("2. Rook\n");
		printf("3. Knight\n");
		printf("4. Bishop\n");
		printf("Choose Option: ");
		scanf("%d", &promotion);
	}
	while(done == 0) {
		switch(promotion) {
			case 1:
				if(Color == 1)
					Board[Rank][File] = wQueen;
				else
					Board[Rank][File] = bQueen;
				done = 1;
				break;
			case 2:
				if(Color == 1)
					Board[Rank][File] = wRook;
				else
					Board[Rank][File] = bRook;
				done = 1;
				break;
			case 3:
				if(Color == 1)
					Board[Rank][File] = wKnight;
				else
					Board[Rank][File] = bKnight;
				done = 1;
				break;
			case 4:
				if(Color == 1)
					Board[Rank][File] = wBishop;
				else
					Board[Rank][File] = bBishop;
				done = 1;
				break;
			default:
				printf("Invalid choice. Please try again...\n");
				break;
		}
	}
}

int QueenMove(int Board[8][8], char Move[5]){
	int Rank, File, newRank , newFile , diffRank, diffFile;
	File = Move[0] - 'a';
	Rank = 7 - (Move[1] - '1');
	newFile = Move[2] - 'a';
	newRank = 7 - (Move[3] - '1');
	diffRank = abs(newRank - Rank);
	diffFile = abs(newFile - File);

	if(diffRank == diffFile)
		return BishopMove(Board, Move);
	else if ((Rank == newRank) || (File == newFile))
		return RookMove(Board, Move);
	else
		return 0;
}

int KingMove(int Board[8][8], char Move[5], int hasKingmoved)
{
	int pieceID = IdentityDetect(Board, Move, 1);
  	int OrX, DesX, OrY, DesY, castle;
  	int diffX = 0;
 	int diffY = 0;
	char RookLoc[5];

	strcpy(RookLoc, Move);
  	OrX = Move[0] - 'a';
 	DesX = Move[2] - 'a';
  	OrY = 7 - (Move[1] - '1');
  	DesY = 7 - (Move[3] - '1');

  	diffX = abs(DesX - OrX);
  	diffY = abs(DesY - OrY);

  	castle = (diffX == 2 && diffY == 0);
	if(hasKingmoved == 0 && castle == 1 && (IdentityDetect(Board, "e1", 1) == wKing || IdentityDetect(Board, "e8", 1) == bKing)){
		RookLoc[2] += 1;
		// White kingside castle
		if(pieceID == 6 && IdentityDetect(Board, RookLoc, 2) == wRook && IdentityDetect(Board, "f1", 1) == 0 && IdentityDetect(Board, "g1", 1) == 0){
			Board[7][5] = Board[7][7];   // white rook moves 2 squares to left
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to g1 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[7][7] = 0; // rook square is empty
			return 1;
		}
		// Black kingside castle
		else if(pieceID == 12 && IdentityDetect(Board, RookLoc, 2) == bRook && IdentityDetect(Board, "f8", 1) == 0 && IdentityDetect(Board, "g8", 1) == 0){
			Board[0][5] = Board[0][7];   // black rook moves 2 squares to left
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to g8 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[0][7] = 0; // rook square is empty
			return 1;
		}
		RookLoc[2] -= 3;
		// White queenside castle
		if(pieceID == 6 && IdentityDetect(Board, RookLoc, 2) == wRook && IdentityDetect(Board, "b1", 1) == 0 && IdentityDetect(Board, "c1", 1) == 0 && IdentityDetect(Board, "d1", 1) == 0){
			Board[7][3] = Board[7][0];   // white rook moves 3 squares to right
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to c1 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[7][0] = 0; // rook square is empty
			return 1;
		}
		// Black queenside castle
		else if(pieceID == 12 && IdentityDetect(Board, RookLoc, 2) == bRook && IdentityDetect(Board, "b8", 1) == 0 && IdentityDetect(Board, "c8", 1) == 0 && IdentityDetect(Board, "d8", 1) == 0){
			Board[0][3] = Board[0][0];   // black rook moves 3 squares to right
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to c8 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[0][0] = 0; // rook square is empty
			return 1;
		}
	}

  	// legal move: King move one space in any direction
  	if((diffX == 1 && diffY == 1) || (diffX == 0 && diffY == 1) || (diffX == 1 && diffY == 0)){
    	Board[DesY][DesX] = Board[OrY][OrX];
    	Board[OrY][OrX] = 0;
    	return 1; // success message code. Move is finished, Board is modified
    }
  	// illegal move
  	else
      return 0; // error message code - this move is not belonging to King
}

int RookMove(int Board[8][8], char Move[5]){
	int OrX, DesX, OrY, DesY;
	int x = 0;
	int y = 0;

	OrX = Move[0] - 'a';
	DesX = Move[2] - 'a';
	OrY = 7 - (Move[1] - '1');
	DesY = 7 - (Move[3] - '1');

	// legal move: Rook move in same row or in same column
	if((OrX == DesX) || (OrY == DesY))
	{
	 	// move vertically
		if(OrX == DesX)
		{
			if(OrY < DesY)
 			{
 				for(y = OrY + 1; y < DesY; y++)
				{
 					if(Board[y][OrX] != 0)
						break;
				}
			}
			else if(OrY > DesY)
			{
				for(y = OrY - 1; y > DesY; y--)
				{
					if(Board[y][OrX] != 0)
					break;
				}
			}

			// there is an obstacle in the path
			if(y != DesY)
			{
				return -1; // error message code - obstacle in path from Origin to Dest
			}
			// the path is clean
			else
			{
				Board[DesY][DesX] = Board[OrY][OrX];
				Board[OrY][OrX] = 0;
				return 1; // success message code. Move is finished, Board is modified
			}
        	}
		// move horizontally
		else if(OrY == DesY)
		{
			if(OrX < DesX)
			{
				for(x = OrX + 1; x < DesX; x++)
				{
					if(Board[OrY][x] != 0)
						break;
				}
			}
			else if(OrX > DesX)
			{
				for(x = OrX - 1; x > DesX; x--)
				{
					if(Board[OrY][x] != 0)
					break;
				}
			}
	
			// there is an obstacle in the path
			if(x != DesX)
			{
				return -1; // error message code - obstacle in path from Origin to Dest
			}
			// the path is clean
			else
			{
				Board[DesY][DesX] = Board[OrY][OrX];
				Board[OrY][OrX] = 0;
				return 1; // success message code. Move is finished, Board is modified
			}
		}
	}
	// illegal move - not vertical or horizontal
	return 0; // error message code - this move is not belonging to Rook
}

int winCon(int Board[8][8], MLIST *MoveList, int count){
  int checkStatus = canCaptureKing(Board, count), validMoveCount = 0, Identity, desIdentity, successfulMove, testCheckStatus;
	int tempBoard[8][8];
	char testMove[5];
    testMove[4] = '\0';
	char* testOrColor; 
	char* testDesColor;
	
	// This offsets the piece number to look for
	// For white's turn, we want to move white pieces (+0) and for black's turn, we want to move black pieces (+6)
	int pieceOffset = count % 2 == 1 ? 0 : 6;

	for (int Rank = 0; Rank < 8; Rank++){
		for (int File = 0; File < 8; File++){
			// If piece is on our side, try to move it
			testMove[0] = 'h' - File;
			testMove[1] = '8' - Rank;
			if (IdentityDetect(Board, testMove, 1) >= wPawn + pieceOffset && IdentityDetect(Board, testMove, 1) <= wKing + pieceOffset){
				for (int destRank = 0; destRank < 8; destRank++){
					for (int destFile = 0; destFile < 8; destFile++){
						testMove[2] = 'h' - destFile;
						testMove[3] = '8' - destRank;
						for(int y = 0; y < 8; y++) {
							for(int x = 0; x < 8; x++) {
								tempBoard[y][x] = Board[y][x];
							}
						}

						Identity = IdentityDetect(tempBoard, testMove, 1);
						desIdentity = IdentityDetect(tempBoard, testMove, 2);
						pieceColor(Identity, &testOrColor);
						pieceColor(desIdentity, &testDesColor);

						if (*testOrColor != *testDesColor) {
							successfulMove = pieceMove(tempBoard, testMove, 0, MoveList, 0);
							if (successfulMove == 1) {
								testCheckStatus = canCaptureKing(tempBoard, count);
								if (testCheckStatus == 0){
									validMoveCount++;
									strcpy(globalValidAIMove, testMove);
									return checkStatus;
								}
							}
						}
					}
				}
			}
		}
	}

	if (checkStatus == 2 && validMoveCount == 0) // Black is checkmated; White wins
		return 3;
	if (checkStatus == 1 && validMoveCount == 0) // White is checkmated; Black wins
		return 4;
	if (checkStatus == 0 && validMoveCount == 0) // Stalemate
		return 5;
	
	return checkStatus;
}

// We only return a value (either 1 or 2) if THAT side's king is in check
int canCaptureKing(int Board[8][8], int count){
  int x = 0;
  int y = 0;
  int king;
  int checkStatus;

  // after white makes a move
  // check if black king is in check
  if(count % 2 == 0)
    {
      king = bKing;
      checkStatus = 2;
    }
  // after black makes a move
  // check if white king is in check
  else if(count % 2 == 1)
    {
      king = wKing;
      checkStatus = 1;
    }
  
  // detect white/black king position
  for(x = 0; x < 8; x++)
    {
      for(y = 0; y < 8; y++)
	{
	  if(Board[x][y] == king)
	    break;
	}
      if(Board[x][y] == king)
	break;
    }
  
  // after white makes a move
  // detect any piece attacks black king
  if(count % 2 == 0)
    {
     if(OppKing(Board, wKing, x, y) || OppBishop(Board, wBishop, x, y) ||
	 OppRook(Board, wRook, x, y) || OppQueen(Board, wQueen, x, y)   ||
	 OppPawn(Board, wPawn, x, y) || OppKnight(Board, wKnight, x, y)  )
	return checkStatus;
    }
  // after black makes a move
  // detect any piece attacks white king
  else if(count % 2 == 1)
    {
     if(OppKing(Board, bKing, x, y) || OppBishop(Board, bBishop, x, y) ||
	 OppRook(Board, bRook, x, y) || OppQueen(Board, bQueen, x, y)   ||
	 OppPawn(Board, bPawn, x, y) || OppKnight(Board, bKnight, x, y)  )
	return checkStatus;
    }

  return 0;
}

int pieceMove(int Board[8][8], char Move[5], int hasKingmoved, MLIST *MoveList, int promotionAllowed) {
	int pieceID = IdentityDetect(Board, Move, 1);
	int result;
	
	int Color = IdentityDetect(Board, Move, 1);
	int desColor = IdentityDetect(Board, Move, 2);
	if(Color == desColor) {
		result = 0;
	} else {
		if (pieceID == wPawn || pieceID == bPawn)
			result = PawnMove(Board, Move, MoveList, promotionAllowed);
		else if (pieceID == wKnight || pieceID == bKnight)
			result = KnightMove(Board, Move);
		else if (pieceID == wBishop || pieceID == bBishop)
			result = BishopMove(Board, Move);
		else if (pieceID == wRook || pieceID == bRook)
			result = RookMove(Board, Move);
		else if (pieceID == wQueen || pieceID == bQueen)
			result = QueenMove(Board, Move);
		else if (pieceID == wKing || pieceID == bKing)
			result = KingMove(Board, Move, hasKingmoved);
	}

	return result;
}

//Takeback move
void Takeback(int Board[8][8], MLIST *MoveList){
	int orX;
	int desX;
	int orY;
	int desY;
	int diffX;
	int diffY;

	MOVE *lm = RemoveLastMove(MoveList);

	orX = lm->NewMove[0] - 'a';		    
	desX = lm->NewMove[2] - 'a';
	orY = 7 - (lm->NewMove[1] - '1');
	desY = 7 - (lm->NewMove[3] - '1');
	diffX = abs(desX - orX);
	diffY = abs(desY - orY);

	Board[orY][orX] = lm->OriPiece;
	Board[desY][desX] = lm->DesPiece;

	if((lm->OriPiece == wPawn) && (lm->DesPiece == 0) && (diffX == 1) && (diffY == 1))
	  {
	    Board[desY + 1][desX] = bPawn;
	  }
	if((lm->OriPiece == bPawn) && (lm->DesPiece == 0) && (diffX == 1) && (diffY == 1))
	  {
	    Board[desY - 1][desX] = wPawn;
	  }

	DeleteMove(lm);
}

int IndexCheck(int x, int y)
{
  int result;

  result = (x >= 0 && x < 8 && y >= 0 && y < 8) ? 1 : 0;

  return result;
}

int OppKing(int Board[8][8], int Piece, int x, int y)
{
  int Rank = 0;
  int File = 0;

  // up-left diagonal                                                                                                                                                                                                                   
  Rank = x - 1;
  File = y - 1;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  // up                                                                                                                                                                                                                                 
  Rank = x - 1;
  File = y;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  // up-right diagonal                                                                                                                                                                                                                  
  Rank = x - 1;
  File = y + 1;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  // left                                                                                                                                                                                                                               
  Rank = x;
  File = y - 1;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  // right                                                                                                                                                                                                                              
  Rank = x;
  File = y + 1;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  // down-left diagonal                                                                                                                                                                                                                 
  Rank = x + 1;
  File = y - 1;
  if(IndexCheck(Rank, File))
    {
      if(Board[Rank][File] == Piece)
	return 1;
    }

  // down                                                                                                                                                                                                                               
  Rank = x + 1;
  File = y;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  // down-right diagonal                                                                                                                                                                                                                
  Rank = x + 1;
  File = y + 1;
  if(IndexCheck(Rank, File) && Board[Rank][File] == Piece)
    {
      return 1;
    }

  return 0;
}

int OppBishop(int Board[8][8], int Piece, int x, int y)
{
  int Rank = x;
  int File = y;
  int i;

  // IV quadrant diagonal                                                                                                                                                                                                               
  i = 1;
  while(IndexCheck(Rank + i, File + i))
    {
      if(Board[Rank + i][File + i] == Piece)
	{
	  return 1;
	}
      if(Board[Rank + i][File + i] != 0)
	{
	  break;
	}
      i++;
    }

  // II quadrant diagonal                                                                                                                                                                                                               
  i = 1;
  while(IndexCheck(Rank - i, File - i))
    {
      if(Board[Rank - i][File - i] == Piece)
	{
	  return 1;
	}
      if(Board[Rank - i][File - i] != 0)
	{
	  break;
	}
      i++;
    }

  // I quadrant diagonal                                                                                                                                                                                                                
  i = 1;
  while(IndexCheck(Rank - i, File + i))
    {
      if(Board[Rank - i][File + i] == Piece)
	{
	  return 1;
	}
      if(Board[Rank - i][File + i] != 0)
	{
	  break;
	}
      i++;
    }

  // III quadrant diagonal                                                                                                                                                                                                              
  i = 1;
  while(IndexCheck(Rank + i, File - i))
    {
      if(Board[Rank + i][File - i] == Piece)
	{
	  return 1;
	}
      if(Board[Rank + i][File - i] != 0)
	{
	  break;
	}
      i++;
    }

  return 0;
}

int OppRook(int Board[8][8], int Piece, int x, int y)
{
  int Rank = x;
  int File = y;
  int i;

  // Up                                                                                                                                                                                                                                 
  i = 1;
  while(IndexCheck(Rank - i, File))
    {
      if(Board[Rank - i][File] == Piece)
	{
	  return 1;
	}
      if(Board[Rank - i][File] != 0)
	{
	  break;
	}
      i++;
    }

  // Down                                                                                                                                                                                                                               
  i = 1;
  while(IndexCheck(Rank + i, File))
    {
      if(Board[Rank + i][File] == Piece)
	{
	  return 1;
	}
      if(Board[Rank + i][File] != 0)
	{
	  break;
	}
      i++;
    }

  // Left                                                                                                                                                                                                                               
  i = 1;
  while(IndexCheck(Rank, File - i))
    {
      if(Board[Rank][File - i] == Piece)
	{
	  return 1;
	}
      if(Board[Rank][File - i] != 0)
	{
	  break;
	}
      i++;
    }

  // Right                                                                                                                                                                                                                              
  i = 1;
  while(IndexCheck(Rank, File + i))
    {
      if(Board[Rank][File + i] == Piece)
	{
	  return 1;
	}
      if(Board[Rank][File + i] != 0)
	{
	  break;
	}
      i++;
    }

  return 0;
}

int OppQueen(int Board[8][8], int Piece, int x, int y)
{
  int combine = OppBishop(Board, Piece, x, y) || OppRook(Board, Piece, x, y);

  if(combine == 1)
    {
      return 1;
    }

  return 0;
}

int OppKnight(int Board[8][8], int Piece, int x, int y)
{
  int Rank = x;
  int File = y;

  if((IndexCheck(Rank - 2, File - 1) && Board[Rank - 2][File - 1] == Piece) ||
     (IndexCheck(Rank - 1, File - 2) && Board[Rank - 1][File - 2] == Piece) ||
     (IndexCheck(Rank + 1, File - 2) && Board[Rank + 1][File - 2] == Piece) ||
     (IndexCheck(Rank + 2, File - 1) && Board[Rank + 2][File - 1] == Piece) ||
     (IndexCheck(Rank + 2, File + 1) && Board[Rank + 2][File + 1] == Piece) ||
     (IndexCheck(Rank + 1, File + 2) && Board[Rank + 1][File + 2] == Piece) ||
     (IndexCheck(Rank - 1, File + 2) && Board[Rank - 1][File + 2] == Piece)  )
    return 1;

  return 0;
}

int OppPawn(int Board[8][8], int Piece, int x, int y)
{
  int Rank = x;
  int File = y;

  if(Piece == wPawn)
    {
      // white pawn                                                                                                                                                                                                                 
    if((IndexCheck(Rank + 1, File - 1) && Board[Rank + 1][File - 1] == Piece) || (IndexCheck(Rank + 1, File + 1) && Board[Rank + 1][File + 1] == Piece))
	return 1;
    }
  else if (Piece == bPawn)
    {
      // black pawn                                                                                                                                                                                                                 
    if((IndexCheck(Rank - 1, File - 1) && Board[Rank - 1][File - 1] == Piece) || (IndexCheck(Rank - 1, File + 1) && Board[Rank - 1][File + 1] == Piece))
	return 1;
    }
  return 0;
}
