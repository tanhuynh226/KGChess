/*  Move.h   */
#ifndef MOVE_H
#define MOVE_H

#include "MoveList.h"

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

int IdentityDetect(int Board[8][8], char Move[5], int specifier);
void pieceColor(int Identity, char **Color);
int PawnMove(int Board[8][8], char Move[5], MLIST *MoveList, int promotionAllowed);
void Promotion(int Board[8][8], int Color, int Rank, int File, int AI);
int KingMove(int Board[8][8], char Move[5], int hasKingmoved); 
int QueenMove(int Board[8][8], char Move[5]);
int RookMove(int Board[8][8], char Move[5]); 
int BishopMove(int Board[8][8], char Move[5]);
int KnightMove(int Board[8][8], char Move[5]);
int pieceMove(int Board[8][8], char Move[5], int hasKingmoved, MLIST *MoveList, int promotionAllowed);
int winCon(int Board[8][8], MLIST *MoveList, int count);
int canCaptureKing(int Board[8][8], int count);
void Takeback(int Board[8][8], MLIST *MoveList);

int IndexCheck(int x, int y);

int OppKing(int Board[8][8], int Piece, int x, int y);

int OppBishop(int Board[8][8], int Piece, int x, int y);

int OppRook(int Board[8][8], int Piece, int x, int y);

int OppQueen(int Board[8][8], int Piece, int x, int y);

int OppKnight(int Board[8][8], int Piece, int x, int y);

int OppPawn(int Board[8][8], int Piece, int x, int y);

#endif
