#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum piece {
    wPawn = 1,
    wKnight,
    wBishop,
    wRook,
    wQueen,
    wKing,
    bPawn,
    bKnight,
    bBishop,
    bRook,
    bQueen,
    bKing
};

void print_fun(int Board[8][8]);

int IdentityDetect(int Board[8][8], char Move[5], int specifier);

int RookMove(int Board[8][8], char Move[5]);

int KingMove(int Board[8][8], char Move[5], int hasKingmoved);

int BishopMove(int Board[8][8], char Move[5]);

int PawnMove(int Board[8][8], char Move[5]);

void Promotion(int Board[8][8], int Rank, int File);

int KnightMove(int Board[8][8], char Move[5]);

int QueenMove(int Board[8][8], char Move[5]);

int main()
{
  		int Board[8][8]={
			{10,  8,  9, 11,  0,  9,  8,  0},
			{ 7,  7,  7,  7,  7,  0,  7,  7},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 0,  0,  0,  0, 12,  0,  0,  0},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 0,  0,  0,  0,  0,  0,  0,  0},
			{ 1,  1,  0,  0,  0,  1,  5,  1},
			{ 4,  2,  3,  5,  6,  3,  2,  4}
		};

		char Move[4];
		int cont = 1;
		int result;

		print_fun(Board);

		while(cont)
		  {
		    printf("Please enter your move: ");
		    scanf("%s", Move);
		    getchar();
		    printf("Your move is: %s\n", Move);
		    result = QueenMove(Board, Move);

		    if(result == 1)
				print_fun(Board);
		    else if(result == 0)
		      printf("Illegal move.\n");
		    else
		      printf("The path is blocked by other pieces.\n");

		    printf("Do you want to continue? 1 - yes, 0 - no\n");
		    scanf("%d", &cont);

		    getchar();
		  }
		return 0;
}

void print_fun(int Board[8][8])
{
		printf("  +----+----+----+----+----+----+----+----+\n");
		for(int i = 0; i < 8; i++)
		  {
		    printf("%d |", (8 - i));
		    for(int j = 0; j < 8; j++)
		      {
			if(Board[i][j] == 10)
			  printf(" bR |");
			else if(Board[i][j] == 8)
			  printf(" bN |");
			else if(Board[i][j] == 9)
			  printf(" bB |");
			else if(Board[i][j] == 11)
			  printf(" bQ |");
			else if(Board[i][j] == 12)
			  printf(" bK |");
			else if(Board[i][j] == 7)
			  printf(" bP |");
			else if(Board[i][j] == 4)
			  printf(" wR |");
			else if(Board[i][j] == 2)
			  printf(" wN |");
			else if(Board[i][j] == 3)
			  printf(" wB |");
			else if(Board[i][j] == 5)
			  printf(" wQ |");
			else if(Board[i][j] == 6)
			  printf(" wK |");
			else if(Board[i][j] == 1)
			  printf(" wP |");
			else if(Board[i][j] == 0)
			  printf("    |");
		      }
		    printf("\n");
		    printf("  +----+----+----+----+----+----+----+----+\n");
		  }
		printf("     a    b    c    d    e    f    g    h  \n");
}

int IdentityDetect(int Board[8][8], char Move[5], int specifier){
    if (specifier == 1) {
        int File = Move[0] - 'a';
        int Rank = 7 - (Move[1] - '1');
        int pieceID = Board[Rank][File];
        return pieceID;
    }
    else if (specifier == 2) {
        int File = Move[2] - 'a';
        int Rank = 7 - (Move[3] - '1');
        int pieceID = Board[Rank][File];
        return pieceID;
    }
}

int RookMove(int Board[8][8], char Move[5])
{
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
	      return -1; // error message code
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
  // illegal move
  else
    {
      return 0; // error message code - this move is not belonging to Rook
    }
}

int KingMove(int Board[8][8], char Move[5], int hasKingmoved)
{
	int pieceID = IdentityDetect(Board, Move, 1);
	int pieceID2 = IdentityDetect(Board,Move, 2);
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
	if(hasKingmoved == 0 && castle == 1 && (IdentityDetect(Board, "e1", 1) == 6 || IdentityDetect(Board, "e8", 1) == 12)){
		RookLoc[2] += 1;
		// White kingside castle
		if(pieceID == 6 && IdentityDetect(Board, RookLoc, 2) == 4 && IdentityDetect(Board, "f1", 1) == 0 && IdentityDetect(Board, "g1", 1) == 0){
			Board[7][5] = Board[7][7];   // white rook moves 2 squares to left
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to g1 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[7][7] = 0; // rook square is empty
			return 1;
		}
		// Black kingside castle
		else if(pieceID == 12 && IdentityDetect(Board, RookLoc, 2) == 10 && IdentityDetect(Board, "f8", 1) == 0 && IdentityDetect(Board, "g8", 1) == 0){
			Board[0][5] = Board[0][7];   // black rook moves 2 squares to left
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to g8 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[0][7] = 0; // rook square is empty
			return 1;
		}
		RookLoc[2] -= 3;
		// White queenside castle
		if(pieceID == 6 && IdentityDetect(Board, RookLoc, 2) == 4 && IdentityDetect(Board, "b1", 1) == 0 && IdentityDetect(Board, "c1", 1) == 0 && IdentityDetect(Board, "d1", 1) == 0){
			Board[7][3] = Board[7][0];   // white rook moves 3 squares to right
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to c1 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[7][0] = 0; // rook square is empty
			return 1;
		}
		// Black queenside castle
		else if(pieceID == 12 && IdentityDetect(Board, RookLoc, 2) == 10 && IdentityDetect(Board, "b8", 1) == 0 && IdentityDetect(Board, "c8", 1) == 0 && IdentityDetect(Board, "d8", 1) == 0){
			Board[0][3] = Board[0][0];   // black rook moves 3 squares to right
			Board[DesY][DesX] = Board[OrY][OrX]; // king goes to c8 
			Board[OrY][OrX] = 0; // king square is empty 
			Board[0][0] = 0; // rook square is empty
			return 1;
		}
	}

  	// legal move:  wKing move one space in any direction

  	if((diffX == 1 && diffY == 1) || (diffX == 0 && diffY == 1) || (diffX == 1 && diffY == 0)) { 
    	Board[DesY][DesX] = Board[OrY][OrX];
    	Board[OrY][OrX] = 0;
    	return 1; // success message code. Move is finished, Board is modified
    }
  	// illegal move
  	else
      return 0; // error message code - this move is not belonging to King

	
}
int BishopMove(int Board[8][8], char Move[5])
{
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

int PawnMove(int Board[8][8], char Move[5]){
	int Rank, File, newRank , newFile , diffRank, diffFile;  //Rank = Row, File = Column
	Rank = 7 - (Move[1] - '1'); // Mapping Row character into a index value 
	File = Move[0] - 'a';       // Mapping Column character into a index value  
	newRank = 7 - (Move[3] - '1'); // Mapping final position
	newFile = Move[2] - 'a';
	diffRank = abs(Rank - newRank); //differnce between the intial and final position (Rank)
	diffFile = abs(File - newFile);//difference between the inital and final position (File)

	int Color = IdentityDetect(Board, Move, 1);
	if(Color == 1 && diffRank == 1 && newRank < Rank && diffFile == 0 && IdentityDetect(Board, Move, 2) == 0) { //Checks that color is black and new row is smaller then current row to go forward
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 0)
			Promotion(Board, newRank, newFile);
		return 1;
	}
	if(Color == 7 && diffRank == 1 && newRank > Rank && diffFile == 0 && IdentityDetect(Board, Move, 2) == 0) { //Checks that color is white and new row is larger then current row to go forward
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 7)
			Promotion(Board, newRank, newFile);
		return 1;
	}
	if(Color == 1 && diffRank == 2 && newRank < Rank && diffFile == 0 && IdentityDetect(Board, Move, 2) == 0 && Rank == 6) { //White pawn can move 2 squares forward from its initial position
		Move[3] -= 1;
		if (IdentityDetect(Board, Move, 2) == 0){
			Move[3] += 1;
			Board[newRank][newFile] = Board[Rank][File];
			Board[Rank][File] = 0;
			return 1;
		}
		else
			return -1;
	}
	if(Color == 7 && diffRank == 2 && newRank > Rank && diffFile == 0 && IdentityDetect(Board, Move, 2) == 0 && Rank == 1) { //Black pawn can move 2 squares forward from its initial position
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
	if(Color == 1 && diffRank == 1 && diffFile == 1 && newRank < Rank && IdentityDetect(Board, Move, 2) > 6){ //checks that change and column are same value (kill move)
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 0)
			Promotion(Board, newRank, newFile);
		return 1;		
	}
	if(Color == 7 && diffRank == 1 && diffFile == 1 && newRank > Rank && IdentityDetect(Board, Move, 2) < 7 && IdentityDetect(Board, Move, 2) != 0){ //checks that change and column are same value (kill move)
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		if(newRank == 7)
			Promotion(Board, newRank, newFile);
		return 1;		
	}
	Move[3] -= 1;
	if(Color == 1 && Rank == 3 && diffFile == 1 && newRank < Rank && newRank == 2 && IdentityDetect(Board, Move, 2) == 7){ //En passant for white
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		Board[Rank][File-1] = 0;
		return 1;
	}
	Move[3] += 2;
	if(Color == 7 && Rank == 4 && diffFile == 1 && newRank > Rank && newRank == 5 && IdentityDetect(Board, Move, 2) == 1){ //En Passant for black
		Board[newRank][newFile] = Board[Rank][File];
		Board[Rank][File] = 0;
		Board[Rank][File+1] = 0;
		return 1;	 
	}
	return 0;
}

void Promotion(int Board[8][8], int Rank, int File){
	int promotion, done = 0;
	printf("\nWhat piece would you like to promote to?\n");
	printf("1. Queen\n");
	printf("2. Rook\n");
	printf("3. Knight\n");
	printf("4. Bishop\n");
	printf("Choose Option: ");
	scanf("%d", &promotion);
	while(done == 0) {
		switch(promotion) {
			case 1:
				Board[Rank][File] = 5;
				done = 1;
				break;
			case 2:
				Board[Rank][File] = 4;
				done = 1;
				break;
			case 3:
				Board[Rank][File] = 2;
				done = 1;
				break;
			case 4:
				Board[Rank][File] = 3;
				done = 1;
				break;
			default:
				printf("Invalid choice. Please try again...\n");
				break;
		}
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