/* Client.c */

#include "Client.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <netdb.h>
#include "Move.h"
#include "MoveLog.h"
#include "MoveList.h"
#include "Board.h"

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

int Exit_Undo(char uInpt[5], int count, int Board[8][8], MLIST *Move_List){
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

void error(const char *Program, const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!", stderr);
    exit(20);
} 

void client(int argc, char *argv[], int Board[8][8])
{
    int l, n, player = 2, login = 0, win = 0, count;
    int SocketFD = 0,	/* socket file descriptor */
	PortNo;		/* port number */
    struct sockaddr_in
	ServerAddress;	/* server address we connect with */
    struct hostent
	*Server;	/* server host */
    char SendBuf[256];	/* message buffer for sending a message */
    char RecvBuf[256];	/* message buffer for receiving a response */

    char uInpt[5];      /* user input */

    MLIST *MoveList;    /* list to store move */
    MOVE *move;         /* move node */

	/* Create a new MoveList */
	MoveList = NewMoveList();

    int orR = 0;        /* origin row */
    int desR = 0;       /* destination row */
    int orC = 0;        /* origin column */
    int desC = 0;       /* destination column */
    char* OrColor;

    int Identity;       /* piece at origin */
    int desIdentity;    /* piece at destination */
    int result;         /* value return from pieceMove() */

    int hasWKingmoved = 0;
    int hasBKingmoved = 0;
    //printf("%s: Starting...\n", argv[0]);
    if (argc < 3)
    {   fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
	exit(10);
    }

    //printf("%s: Looking up host %s on the network...\n", argv[0], argv[1]);

    Server = gethostbyname(argv[1]);
    if (Server == NULL)
    {   fprintf(stderr, "%s: no such host named '%s'\n", argv[0], argv[1]);
        exit(10);
    }
    PortNo = atoi(argv[2]);

    //printf("%s: Using port %d...\n", argv[0], PortNo);

    if (PortNo < 12000 || PortNo > 12099)
    {   fprintf(stderr, "%s: invalid port number %d, should be between 12000 and 12099\n",
		argv[0], PortNo);
        exit(10);
    }

    //printf("%s: Preparing the server address...\n", argv[0]);

    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];
    printf("%s: Connecting to the server...\n", argv[0]);
    
    do{
    if (!SocketFD){
        SocketFD = socket(AF_INET, SOCK_STREAM, 0);
        if (connect(SocketFD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
            error(argv[0], "connecting to server failed");
        if(login == 0){
		    printf(
		    "%s: enter 'login' followed by username and password to log in to an existing account,\n"
		    "             \"login [username] [password]\"\n"
		    "             or 'register' followed by username and password to register for a new account,\n"
		    "             \"register [username] [password]\"\n"
            "             or 'quit' to disconnect:\n"
		    "command: ", argv[0]);
		    fgets(SendBuf, sizeof(SendBuf), stdin);
	    }   
        l = strlen(SendBuf);
	    if (SendBuf[l-1] == '\n')
		    SendBuf[--l] = 0;
        if (l){
	        write(SocketFD, SendBuf, l);
        }
    } 

	//printf("%s: Waiting for response...\n", argv[0]);

	n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
	if (n < 0) 
		error(argv[0], "reading from socket failed");
	RecvBuf[n] = 0;
	//printf("%s: Received response: %s\n", argv[0], RecvBuf);
	if(0 == strncmp(RecvBuf, "move", 4)){
		for(int i = 0; i < 5; i++){
			uInpt[i] = RecvBuf[i + 5];
		}
		orR = 7 - (uInpt[1] - '1');
		desR = 7 - (uInpt[3] - '1');
		orC = uInpt[0] - 'a';
		desC = uInpt[2] - 'a';
		Identity = IdentityDetect(Board, uInpt, 1);
		desIdentity = IdentityDetect(Board, uInpt, 2);
		if(Identity == wKing)
			result = pieceMove(Board, uInpt, hasWKingmoved++, MoveList, 1);
		if(desIdentity == bKing)
			result = pieceMove(Board, uInpt, hasBKingmoved++, MoveList, 1);
		else
			result = pieceMove(Board, uInpt, 0, MoveList, 1);
		move = NewMove(Identity, desIdentity, uInpt);
		AppendMove(MoveList, move);
        if (MoveList->Length > 2 && player == 0)
            win = winCon(Board, MoveList, count);
        else if (MoveList->Length > 2 && player == 1)
            win = winCon(Board, MoveList, count + 1);
        printWin(win);
        if (win == 3 || win == 4 || win == 5) {
            print_fun(Board);
            break;
        }
        count++;
	}
	else if (0 == strcmp(RecvBuf, "Invalid info")){
		printf("Username/password was incorrect! Please try again!\n");
        close(SocketFD);
        SocketFD = 0;
    }
	else if (0 == strcmp(RecvBuf, "Valid info")){
		printf("Signed in successfully!\n");
		login++;
	}
	else if (0 == strcmp(RecvBuf, "Account added")){
        printf("Account has been registered!\n");	
        close(SocketFD);
	    SocketFD = 0;
    }
    else if (0 == strcmp(RecvBuf, "Account could not be added")) {
        printf("This account already exists!\n");
        close(SocketFD);
        SocketFD = 0;
    }
    else if (0 == strcmp(RecvBuf, "side white")){
        printf("You are white.\n");
        player = 0;
        login++;
        count = 0;
    }
    else if (0 == strcmp(RecvBuf, "side black")){
        printf("You are black.\n");
        player = 1;
        login++;
        count = 1;
    }
    else if (0 == strcmp(RecvBuf, "undo")){
        Takeback(Board, MoveList);
        Takeback(Board, MoveList);
    }
    else if (0 == strcmp(RecvBuf, "Disconnecting...") || 0 == strcmp(RecvBuf,"Server shutting down..."))
        break;

    if (login == 1 && player == 2){ // select side after logging in
        printf(
        "%s: enter 'side white' to play as white\n"
		"             or 'side black' to play as black\n"
        "             or 'quit' to disconnect:\n"
		"command: ", argv[0]);
        fgets(SendBuf, sizeof(SendBuf), stdin); 
    }
	else if (login > 1) { // logged in and has selected side
		print_fun(Board);
		//Handle Chat function
		if(0 == strncmp(RecvBuf, "chat", 4)){
			strncpy(RecvBuf, &RecvBuf[5], sizeof(RecvBuf));
			if ((player == 0 && count % 2 == 0) || (player == 1 && count % 2 == 1))
			    printf("\nWhite: %s\n\n", RecvBuf);
            if ((player == 0 && count % 2 == 1) || (player == 1 && count % 2 == 0))
                printf("\nBlack: %s\n\n", RecvBuf);
		}
		printf(
		"%s: enter 'move' followed by the move to send to the server,\n"
		"             or 'chat' followed by the message,\n"
		"             or 'takeback' to undo last move,\n"
		"             or 'quit' to disconnect,\n"
		"             or 'shutdown' to quit both server and client:\n", argv[0]);
        
        if ((player == 0 || player == 1) && count % 2 == 0) {
            printf("command: ");
		    fgets(SendBuf, sizeof(SendBuf), stdin);
        }
	} 
	
	
	//printf("\n\n Test %s\n\n", SendBuf);

	if(0 == strncmp(SendBuf, "move", 4))
	{
	  /* User input */
	  for(int i = 0; i < 5; i++)
	    {
	      uInpt[i] = SendBuf[i + 5];
	    }
	  
	  //printf("User Input: %s\n", uInpt);
	  orR = 7 - (uInpt[1] - '1');
	  desR = 7 - (uInpt[3] - '1');
	  orC = uInpt[0] - 'a';
	  desC = uInpt[2] - 'a';
      OrColor = "0";

	  Identity = IdentityDetect(Board, uInpt, 1);
	  desIdentity = IdentityDetect(Board, uInpt, 2);
      pieceColor(Identity, &OrColor);
	  if(Identity == 0) /* initial position is empty */
	  {
	      printf("Initial position is empty. Try again.\n");
	      continue;
	  }

	  /* Initial position and destination have the same color */
	  if((Identity <= wKing && desIdentity <= wKing && desIdentity != 0) || (Identity >= bPawn && desIdentity >= bPawn && desIdentity != 0))
	  {
	    if(Identity <= wKing)
	      printf("Illegal move. The piece at %c%c has the same %s color.\nTry again.\n", SendBuf[7], SendBuf[8], "white");
	    else
	      printf("Illegal move. The piece at %c%c has the same %s color.\nTry again.\n", SendBuf[7], SendBuf[8], "black");
	    continue;
	  }
      else if ((player == 0 && *OrColor == 'b') || (player == 1 && *OrColor == 'w')){
        printf("That's not your piece! Try again.\n");
        continue;
      }
	  /* Moving piece and check for special pieces */
	  else
	  { 
	    if(Identity == wKing && player == 0)
	    {
	      result = pieceMove(Board, uInpt, hasWKingmoved++, MoveList, 1);
	    }
	    if(desIdentity == bKing && player == 1)
	    {
	      result = pieceMove(Board, uInpt, hasBKingmoved++, MoveList, 1);
	    }
	    else if ((player == 0 && *OrColor == 'w') || (player == 1 && *OrColor == 'b'))
	    {
	      result = pieceMove(Board, uInpt, 0, MoveList, 1);
	    }
	    
		move = NewMove(Identity, desIdentity, uInpt);
	  	AppendMove(MoveList, move);
		Takeback(Board, MoveList);
		
	    if(result == -1)
	    {
	      printf("Error: obstacle in path from Origin to Dest.\nTry again.\n");
	      continue;
	    }
	    else if(result == 0)
	    {
	      printf("Error: this move does not belong to this piece.\nTry again.\n");
	      continue;
	    }
	  }
	}

    if(SocketFD){
	    l = strlen(SendBuf);
	    if (SendBuf[l-1] == '\n')
		    SendBuf[--l] = 0;
        if (l){   
		    //printf("%s: Sending message '%s'...\n", argv[0], SendBuf);
	        n = write(SocketFD, SendBuf, l);
            bzero(SendBuf, sizeof(SendBuf));
	        if (n < 0)
	            error(argv[0], "writing to socket failed");
	    }
    }

    } while((0 != strncmp("Disconnecting...", RecvBuf, 16)) && (0 != strncmp("Server shutting down...", RecvBuf, 23)));
    DeleteMoveList(MoveList);
    close(SocketFD);
    printf("%s: Exiting...\n\n", argv[0]);
}


