/* Server.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "AI.h"
#include "Board.h"
#include "Move.h"
#include "MoveList.h"
#include "MoveLog.h"
#include "Client.h"

#define BUFFER_SIZE 1000

//Global Variables
int Shutdown = 0;

void error(const char *Program, const char *ErrorMsg)
{
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!", stderr);
    exit(20);
} 

void fileFunction(char* username, char* password)
{
    FILE *out;
    
	//Create and write a file and save
	out = fopen("./bin/database.txt", "a"); // we changed fp to out

    // Can't open file
	if(out == NULL)
		printf("Error! Can't open file!\n");    
    
    // Write to database.txt   
	fprintf(out,"\n%s, %s", username, password);
    fclose(out);
}

int indexOf(FILE *check, const char *word, int *line, int *col)
{
    char str[BUFFER_SIZE];
    char *pos;

    *line = -1;
    *col  = -1;

    while ((fgets(str, BUFFER_SIZE, check)) != NULL)
    {
        *line += 1;

        // Find first occurrence of word in str
        pos = strstr(str, word);

        if (pos != NULL)
        {
            // First index of word in str is 
            // Memory address of pos - memory
            // address of str.
            *col = (pos - str);
            break;
        }
    }

    // If word is not found then set line to -1
    if (*col == -1)
        *line = -1;
    return *col;
} 

int accountVerify(char* username)
{
        char* word = username;
        int line, col;
        int verify = 0;       
       
        FILE *check;
        //Reading file 
        check = fopen("./bin/database.txt","r" ); 
        if(check == NULL)
	    {
		    printf("Error! Couldn't open file!\n"); 
            verify = 2;
	    }  

        // Find index of word in fptr
        indexOf(check, word, &line, &col);

        if (line != -1)
        { 
            // this means theres a username in there already. 
            printf("'%s' found at line: %d, col: %d\n", word, line + 1, col + 1);
            printf("username already taken! Reopen Program and try again!\n");
            verify = 1;
        }

    // Close file
    fclose(check);

    return verify;
} 

//login and password verify
int login(char* username, char* password)
{

		FILE *ptr;
		char compare[256] = "";
		int line, col;

		//read
	    ptr = fopen("./bin/database.txt","r");
        //file cant be read
        
        if(ptr == NULL)
	{
		printf("Error!");
		exit(1);
	}

        //Initialize user input login and password
	strcat(compare, username);
	strcat(compare, ", ");
	strcat(compare, password);

	line = indexOf(ptr, compare, &line, &col);

	if (line != -1)
	{ // this all means theres a user in there already
		printf("%s has succesfully logged in\n", username);
        fclose(ptr);
		return 1;
		exit(1);
    }
    else{
        printf("Username or Password is incorrect\n");
        fclose(ptr);
    	return 0;
    }
}
    
int MakeServerSocket(int argc, char *argv[]){
    int ServSocketFD, PortNo;
    struct sockaddr_in ServerAddress;

	printf("%s: Starting...\n", argv[0]);
    if (argc < 2)
    {   
        fprintf(stderr, "Usage: %s port\n", argv[0]);
	    exit(10);
    }
    PortNo = atoi(argv[1]);
	if (PortNo < 12000 || PortNo > 12099)
    {   
        fprintf(stderr, "%s: invalid port number %d, should be between 12000 and 12099\n",
		argv[0], PortNo);
        exit(10);
    }

    /* create the socket */
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0)
    	error(argv[0], "service socket creation failed");
	memset(&ServerAddress, 0, sizeof(ServerAddress));
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
    if (bind(ServSocketFD, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) < 0)
        error(argv[0], "binding the server to a socket failed");
    printf("%s: Listening on port %d...\n", argv[0], PortNo);
    if (listen(ServSocketFD, 5) < 0)
        error(argv[0], "listening on socket failed");
    return ServSocketFD;
}

void ProcessRequest(int DataSocketFD, char RecvBuf[256], int n){
	int l, verify;
	char SendBuf[256];
    char accountinfo[40];
    char username[20];
    char password[20];
    int separator;
	
	//Cut string off
	RecvBuf[n] = 0;
	printf("Receive message: -%s-\n", RecvBuf);
    l = strlen(SendBuf);

	//Check for specific inputs and set SendBuf
	if(0 == strncmp(RecvBuf, "move", 4) || 0 == strncmp(RecvBuf, "chat", 4)){
		strncpy(SendBuf, RecvBuf, sizeof(SendBuf) - 1);
		SendBuf[sizeof(SendBuf) - 1] = 0;
	} else if(0 == strcmp(RecvBuf, "takeback")){ //Takes back the latest move
		strncpy(SendBuf, "undo", sizeof(SendBuf) - 1);
		SendBuf[sizeof(SendBuf) - 1] = 0;
	} else if (0 == strcmp(RecvBuf, "quit")){ //Disconnects the client
		printf("Received 'bye' message from client.\n");
		strncpy(SendBuf, "Disconnecting...", sizeof(SendBuf)-1);
		SendBuf[sizeof(SendBuf)-1] = 0;
	} else if (0 == strcmp(RecvBuf, "shutdown")){ //Shuts down the server
		Shutdown = 1;
		printf("Received 'shutdown' message from client.\n");
		strncpy(SendBuf, "Server shutting down...", sizeof(SendBuf)-1);
		SendBuf[sizeof(SendBuf)-1] = 0;
    } else if (0 == strcmp(RecvBuf, "side white")) {
        printf("Received 'side white' message from client.\n");
        strncpy(SendBuf, "side white", sizeof(SendBuf)-1);
    } else if (0 == strcmp(RecvBuf, "side black")) {
        printf("Received 'side black' message from client.\n");
        strncpy(SendBuf, "side black", sizeof(SendBuf)-1);
    } else if (0 == strncmp(RecvBuf, "login", 5)) {
        strncpy(accountinfo, &RecvBuf[6], sizeof(accountinfo)-1);
        separator = strcspn(accountinfo, " ");
        strncpy(username, accountinfo, separator);
        username[separator] = '\0';
        strncpy(password, &accountinfo[separator+1], sizeof(password)-1);
	    if(login(username, password) == 1){
		    strncpy(SendBuf, "Valid info", sizeof(SendBuf) - 1);
		    SendBuf[sizeof(SendBuf) - 1] = 0;
	    } else {
		    strncpy(SendBuf, "Invalid info", sizeof(SendBuf) - 1);
		    SendBuf[sizeof(SendBuf) - 1] = 0;
	    }
    } else if (0 == strncmp(RecvBuf, "register", 8)) {
        strncpy(accountinfo, &RecvBuf[9], sizeof(accountinfo)-1);
        separator = strcspn(accountinfo, " ");
        strncpy(username, accountinfo, separator);
        username[separator] = '\0';
        strncpy(password, &accountinfo[separator+1], sizeof(password)-1);        
        verify = accountVerify(username); // verifies if username is inside already.
        if (verify == 0){
            fileFunction(username, password); // this takes in username and password to the filefunction.        
            strcpy(SendBuf, "Account added");
        }
        else
            strcpy(SendBuf, "Account could not be added");
	} else {   
		strncpy(SendBuf, "ERROR unknown command ", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;
      	strncat(SendBuf, RecvBuf, sizeof(SendBuf)-1-strlen(SendBuf));
    }

	l = strlen(SendBuf);
    if (0 != strcmp(RecvBuf, "side white") && 0 != strcmp(RecvBuf, "side black") && 0 != strncmp(RecvBuf, "register", 8) && 0 != strncmp(RecvBuf, "login", 5)) {
        if (DataSocketFD == 4)
            write(DataSocketFD + 1, SendBuf, l);
        else if (DataSocketFD == 5)
            write(DataSocketFD - 1, SendBuf, l);
    }
    write(DataSocketFD, SendBuf, l);
    bzero(SendBuf, sizeof(SendBuf));
}

void ServerLoop(int argc, char *argv[], int ServSocketFD){ 
    int new_socket , client_socket[2] , max_clients = 2 , res, i , sd, valread;  
    int max_sd;
    struct sockaddr_in address; 
    int addrlen = sizeof(address);
         
    char RecvBuf[256];
    fd_set readfds;  
     
    //initialize all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++) {  
        client_socket[i] = 0;  
    }  

    //clear the socket set 
    FD_ZERO(&readfds); 
    
    while(!Shutdown)  
    {  
        //add master socket to set 
        FD_SET(ServSocketFD, &readfds);  
        max_sd = ServSocketFD;  
             
        //add child sockets to set 
        for (i = 0; i < max_clients; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET(sd, &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an incoming signal from one of the sockets
        res = select(max_sd + 1 , &readfds, NULL, NULL, NULL);  
       
        if (res < 0)  
            error(argv[0], "select error");  
             
        //If something happened on the master socket, then its an incoming connection 
        if (FD_ISSET(ServSocketFD, &readfds))  
        {  
            if ((new_socket = accept(ServSocketFD, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)   
                error(argv[0], "data socket creation (accept) failed");
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                    break;  
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        {  
            sd = client_socket[i];       
            if (FD_ISSET(sd, &readfds))  
            {  
                //Check if client is closing
				if ((valread = read(sd, RecvBuf, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd, (struct sockaddr*)&address , (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    close(sd);  
                    client_socket[i] = 0;
                    FD_CLR(sd, &readfds); 
                }  
				else{
				    ProcessRequest(sd, RecvBuf, valread);
                    bzero(RecvBuf, sizeof(RecvBuf));	
               }
            }  
        }
    }
	printf("%s: Received last message from client, closing data connection.\n", argv[0]);
    printf("%s: Shutting down.\n", argv[0]);
    close(ServSocketFD);
}

int main(int argc, char *argv[])
{
	int ServSocketFD = MakeServerSocket(argc, argv);
    ServerLoop(argc, argv, ServSocketFD);	
    return 0;
}


	
