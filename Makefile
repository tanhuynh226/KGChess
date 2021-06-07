# Makefile: KGChess
# 04/15/21 TH Inital creation

# variable definitions
CC = gcc
DEBUG = -g DDEBUG
CFLAGS = -Wall -ansi -std=c99 -c
LFLAGS = -Wall
INCLUDE = -I src -c

# convenience targets
all: bin/KGChess bin/Server

clean:
	rm -f bin/KGChess
	rm -f bin/*.exe
	rm -f bin/*.o
	rm -f bin/*.a
	
cleanall:
	rm -f bin/KGChess
	rm -f bin/*.exe
	rm -f bin/*.o
	rm -f bin/*.a
	rm -f *.c
	rm -f *.md
	rm -r bin
	rm -r doc
	rm -r src

tar:
	tar zcvf Chess_V1.0_src.tar.gz ../Team20

test:
	$(CC) src/pieceTest.c -o pieceTest
	src/pieceTest

memorytest:
	valgrind bin/KGChess --leak-check=full --track-origins=yes

# compilation rules for KGChess
bin/AI.o: src/AI.c src/AI.h
	$(CC) $(CFLAGS) $(INCLUDE) src/AI.c -o bin/AI.o
bin/Board.o: src/Board.c src/Board.h
	$(CC) $(CFLAGS) $(INCLUDE) src/Board.c -o bin/Board.o
bin/Client.o: src/Client.c src/Client.h
	$(CC) $(CFLAGS) $(INCLUDE) src/Client.c -o bin/Client.o
bin/Move.o: src/Move.c src/Move.h
	$(CC) $(CFLAGS) $(INCLUDE) src/Move.c -o bin/Move.o
bin/MoveList.o: src/MoveList.c src/MoveList.h
	$(CC) $(CFLAGS) $(INCLUDE) src/MoveList.c -o bin/MoveList.o
bin/MoveLog.o: src/MoveLog.c src/MoveLog.h
	$(CC) $(CFLAGS) $(INCLUDE) src/MoveLog.c -o bin/MoveLog.o
libmove: bin/Move.o bin/MoveList.o bin/MoveLog.o
	ar rc libmove.a bin/Move.o bin/MoveList.o bin/MoveLog.o
	ranlib libmove.a
bin/KGChess.o: src/KGChess.c
	$(CC) $(CFLAGS) $(INCLUDE) src/KGChess.c -o bin/KGChess.o

# compilation rules for server
bin/Server: src/Server.c
	$(CC) $(LFLAGS) src/Server.c -o bin/Server

# Generate executable
bin/KGChess: bin/AI.o bin/Board.o bin/Client.o libmove bin/KGChess.o
	$(CC) $(LFLAGS) bin/AI.o bin/Board.o bin/Client.o bin/KGChess.o -L. -lmove -lm -o bin/KGChess
