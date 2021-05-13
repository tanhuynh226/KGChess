/*  MoveList.h   */

#ifndef MOVE_LIST_H
#define MOVE_LIST_H

typedef struct MoveList  MLIST;
typedef struct MoveEntry MENTRY;
typedef struct Move      MOVE;

struct MoveList
{
	int    Length;
	MENTRY *First;
	MENTRY *Last;
};

struct MoveEntry
{
	MLIST  *List;
	MENTRY *Next;
	MENTRY *Prev;
	MOVE   *Move;
};

struct Move
{
	int  OriPiece;
        int  DesPiece;
	char NewMove[5];
};

/* allocate a new move record */
MOVE *NewMove(int OriginPiece, int DestinationPiece, char LTMove[5]);

/* delete a move record */
void DeleteMove(MOVE *m);

/* allocate a new move entry */
MENTRY *NewMoveEntry(MOVE *m);

/* delete a move entry */
MOVE *DeleteMoveEntry(MENTRY *e);

/* allocate a new move list */
MLIST *NewMoveList(void);

/* delete a move list */
void DeleteMoveList(MLIST *l);

/* append a move at the end of list */
void AppendMove(MLIST *l, MOVE *m);

/* remove the last move from the list */
MOVE *RemoveLastMove(MLIST *l);
#endif
