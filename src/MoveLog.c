/*  MoveLog.c  */

#include "MoveLog.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void Print2File(MLIST *l, int winner)
{
  int option;
  printf("1. Create a move log text file\n");
  printf("2. Back to main menu\n");
  printf("Choose option: ");
  scanf("%d", &option);
  if (option == 1) {
    char fLog[26] = "./bin/";
    char fName[20];
    char fType[] = ".txt";
    printf("Enter a name for the move log text file (up to 20 characters): ");
    scanf("%s", fName);
    strcat(fLog, fName);
    strcat(fLog, fType);
    FILE *fp; // File pointer

    time_t t; // Time variable
    time(&t);

    // Open file to write
    fp = fopen(fLog, "w+");

    /* Basic Program info */
    fprintf(fp, "#KGChess\n");
    fprintf(fp, "Version: v1.0\n");
    fprintf(fp, "Filename: %s\n", fName);
    fprintf(fp, "Date: %s\n\n", ctime(&t));

    /* Winner */
    fprintf(fp, "#Winner\n");
    if(winner == 3)
        fprintf(fp, "White\n\n\n");
    else if(winner == 4)
        fprintf(fp, "Black\n\n\n");
    else if(winner >= 5)
        fprintf(fp, "Game ended in a draw\n\n\n");
    /* Move Log */
    fprintf(fp, "#MoveLog\n");

    assert(l);

    MENTRY *e = l->First;
    int index = 1;
    int counter = 0;

    fprintf(fp, "%d. ", index);
    while(e)
      {
        if((counter % 2 == 0)  && (counter != 0))
    {
      fprintf(fp, "\n");
      index++;
      fprintf(fp, "%d. ", index);
    }

        fprintf(fp, "%s ", e->Move->NewMove);

        counter++;
        e = e->Next;
      }

    fprintf(fp, "\n\n\n#EOF\n\n");

    fclose(fp);
    printf("%s was stored successfully.\n\n", fName);
  }
  else
    printf("\nGoing back to main menu...\n\n");
}

void MoveLog(char *fName)
{
  char fLog[26] = "./bin/";
  FILE *fp;
  strcat(fLog, fName);
  // Open file
  fp = fopen(fLog, "r");
  if(fp == NULL)
    {
      printf("Cannot open file\n");
      exit(0);
    }
  
  // Read contents from file
  printf("\nOpening %s...\n\n", fName);
  char c = fgetc(fp);
  while(c != EOF)
    {
      printf("%c", c);
      c = fgetc(fp);
    }

  fclose(fp);
}
