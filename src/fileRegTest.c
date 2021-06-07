#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1000


void fileFunction(char* username, char* password)
{

    //char format = "%s, %s\n";
	char* txt_username = username; 
    char* txt_password = password;
	FILE *in;
    FILE *out;

    //Buffer for character string length 
    char buffer_in[256], buffer_out[256];
    
	//Create and write a file and save
	out = fopen("database2.txt","w"); // we changed fp to out


	if(out == NULL)
	{
		printf("Error!");
		exit(1);
	}
    
	// figure out to write username in here:

	fprintf(out,"%s, %s\n", txt_username, txt_password); //outputs string to our buffer
  //  fwrite(buffer_out, sizeof(buffer_out), strlen(buffer_out), out); //writes to file (hopefully) not sure if sizeof(char)

	fclose(out);



  //Reading File
    in = fopen("database2.txt","r" );
    if(in == NULL)
	{
		printf("Error!");
		exit(1);
	}


    
    //if its able to read file, if not it will return NULL
    fscanf(in, "%[^_]s", buffer_in);
    //if(fgets(buffer_in, 256, in) == NULL)
    //{
       // fclose(in);
       // exit(1);
   // }
    printf("%s", buffer_in);
    //string input
    fscanf(in , "%s,\n", buffer_in);

    printf("Read from file: %s, %s\n", txt_username , txt_password);
    
}







 int accountVerify(char* username, char* password )
    {
        FILE *check;
        char* word = "hi";
        int line, col;
        
       
        
        //Reading file for matches between passowrds and usernames and duplicates
        check = fopen("database2.txt","r" ); 
        if(check == NULL)
	    {
		printf("Error!"); 
		exit(1);
	    }  

        // Find index of word in fptr
        indexOf(check, word, &line, &col);

        if (line != -1) {// this all means theres a user in there already
            printf("'%s' Username already taken: %d, col: %d\n", word, line + 1, col + 1);       
            // printf("username already taken! Please choose another one.\n");
            exit(1);
        }
        else{
            printf("'%s'  word does not exist" );
        }

    // Close file
    fclose(check);

    return 0;
}
    //login and passowrd verify
    void login( char* username, char* password)
    {

		FILE *ptr;
		char compare[256] = "";
		int line, col;

		//read
	    ptr = fopen("database2.txt","r");
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
		// printf("username already taken! Please choose another one.\n");
		exit(1);
    }
    else{
        printf("Username or Password is incorrect");
    }

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

int main()
{
	login( "larrenz", "notcool");
	//accountVerify("wave", ""); FUCK U B ETCH
	//fileFunction("butt", "passwordtest");

}