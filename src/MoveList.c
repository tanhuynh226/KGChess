/*  MoveList.c   */

#include "MoveList.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* allocate a new move record */
MOVE *NewMove(int OriginPiece, int DestinationPiece, char LTMove[5])
{
	MOVE *m = NULL;

	// dynamic memory allocate
	m = malloc(sizeof(MOVE));

	// check memory allocate process
	if(!m)
	{
		perror("Out of memory. Aborting...");
		exit(10);
	}

  	// Store previous piece
	m->OriPiece = OriginPiece;
  	m->DesPiece = DestinationPiece;

  	// Store the move
  	for(int i = 0; i < 4; i++)
    	{
      		m->NewMove[i] = LTMove[i];
    	}

  	return m;

} /* end of "NewMove" */

/* delete a move record */
void DeleteMove(MOVE *m)
{
  	// check pointer existence
  	assert(m);

  	// de-allocate dynamic memory
  	free(m);

} /* end of "DeleteMove" */

/* allocate a new move entry */
MENTRY *NewMoveEntry(MOVE *m)
{
	MENTRY *e = NULL;

  	// dynamic memory allocation
  	e = malloc(sizeof(MENTRY));
  
  	// check memory allocate process
  	if(!e)
    	{
      		perror("Out of memory. Aborting...");
      		exit(10);
     	}

	// initialize the struct's members
	e->List = NULL;
	e->Next = NULL;
	e->Prev = NULL;
	e->Move = m;

	// return this new allocated object to the caller
	return e;

} /* end of "NewMoveEntry" */

/* delete a move entry */
MOVE *DeleteMoveEntry(MENTRY *e)
{
	MOVE *m = NULL;

	// check pointer existence
	assert(e);
	m = e->Move;
  
	// de-allocate dynamic memory
	free(e);

	return m;

} /* end of "DeleteMoveEntry" */

/* allocate a new move list */
MLIST *NewMoveList(void)
{
	MLIST *l = NULL;

	// dynamic memory alloation
	l = malloc(sizeof(MLIST));
	if(!l)
	{
		perror("Out of memory. Aborting...\n");
		exit(10);
    	}

  	// initial the struct's members
	l->Length = 0;
	l->First = NULL;
	l->Last = NULL;

	// return this new allocated object to the caller
	return l;

} /* end of "NewMoveList" */

/* delete a move list */
void DeleteMoveList(MLIST *l)
{
	MENTRY *e, *n;
	MOVE *m;

	// check and point to first entry in the list
	assert(l);
	e = l->First;

	// loop until e reach the end of the list & deallocate each entry of the list
	while(e)
	{
		n = e->Next;
		m = DeleteMoveEntry(e);
		DeleteMove(m);
		e = n;
    	}

	// de-allocate dynamic memory
	free(l);

} /* end of "DeleteMoveList" */

/* append a move at the end of list */
void AppendMove(MLIST *l, MOVE *m)
{
	MENTRY *e = NULL;

	// check pointer existence
	assert(l);
	assert(m);

	// create a new move object
	e = NewMoveEntry(m);

	// if list is not empty
	if(l->Last)
    	{
		e->List = l;
		e->Next = NULL;
		e->Prev = l->Last;
		l->Last->Next = e;
		l->Last = e;
    	}
	// if list is empty
	else
	{
		e->List = l;
		e->Next = NULL;
		e->Prev = NULL;
		l->First = e;
		l->Last = e;
    	}
  
	// Increase list length after appending more node
	l->Length++;

} /* end of "AppendMove" */

/* remove the last move from the list */
MOVE *RemoveLastMove(MLIST *l)
{
	MENTRY *e = NULL;

	// check pointer existence
	assert(l);

	// if list is not empty
	if(l->Last)
	{
		e = l->Last;
		l->Last = e->Prev;
      
		// if list has more than one elements
		if(l->Last)
		{
			l->Last->Next = NULL;
		}
		// if list has only one element
		else
		{
			assert(l->First == e);
			l->First = NULL;
		}
      
		// reduce list size after deleting an element
		l->Length--;
		return DeleteMoveEntry(e);
	}
	// if list is empty
	else
	{
		return NULL;
	}

} /* end of "RemoveLastMove" */
