/* Client.h */
#ifndef CLIENT_H
#define CLIENT_H

#include "Move.h"
#include "MoveLog.h"
void loginmenu(void);
void My_User_Input_Game(int count, char uInpt[5]);
void printWin(int win);
int Exit_Undo(char uInpt[5],int count, int Board[8][8], MLIST *Move_List);
void error(const char *Program, const char *ErrorMsg);
void client(int argc, char *argv[], int Board[8][8]);
MLIST *Human_Vs_Human(int Board[8][8]);

#endif
