# Makefile: KGChess
# 04/15/21 TH Inital creation

# variable definitions
CC = gcc
DEBUG = -g DDEBUG
CFLAGS = -Wall -ansi -std=c99 -c
LFLAGS = -Wall

# convenience targets
all: KGChess

clean:
	rm -f KGChess
	rm -f *.exe
	rm -f *.o
	
cleanall:
	rm -f KGChess
	rm -f *.exe
	rm -f *.o
	rm -f *.c
	rm -f *.a

test:
	$(CC) pieceTest.c -o pieceTest
	./pieceTest

memorytest: 
	valgrind ./KGChess --leak-check=full --track-origins=yes

# compilation rules for KGChess
AI.o: AI.c AI.h
	$(CC) $(CFLAGS) AI.c -o AI.o
Board.o: Board.c Board.h
	$(CC) $(CFLAGS) Board.c -o Board.o
Move.o: Move.c Move.h
	$(CC) $(CFLAGS) Move.c -o Move.o
MoveList.o: MoveList.c MoveList.h
	$(CC) $(CFLAGS) MoveList.c -o MoveList.o
MoveLog.o: MoveLog.c MoveLog.h
	$(CC) $(CFLAGS) MoveLog.c -o MoveLog.o
libmove: Move.o MoveList.o MoveLog.o
	ar rc libmove.a Move.o MoveList.o MoveLog.o
	ranlib libmove.a
KGChess.o: KGChess.c
	$(CC) $(CFLAGS) KGChess.c -o KGChess.o

# Generate executable
KGChess: AI.o Board.o libmove KGChess.o
	$(CC) $(LFLAGS) AI.o Board.o KGChess.o -L. -lmove -lm -o KGChess
