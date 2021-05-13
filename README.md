# KGChess 
## Version 1.0

A chess game coded in C

Please see INSTALL.md for installation instructions.

### Features
* Human vs. Human, Human vs. AI, AI vs. AI
* Chess board implementation
* Piece rules including special moves (Promotion, castling, en passant)
* Check, checkmate, & stalemate condition checking
* Takeback moves
* Ability to store and view game log text file

### Commands
#### Compile game executable
    make all
#### Run KGChess
    bin/KGChess
#### Test with Valgrind
    make memorytest
#### Clean object, library, & executable files
    make clean
#### Complete uninstall
    make cleanall
