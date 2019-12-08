#ifndef _GAMELIB_H_
#define _GAMELIB_H

#include "structlib.h"
#include <stdio.h>
#include <stdlib.h>

/**
 *  Biblioteka za logiku igre
*/

gameBoard gameBoardInit(float x, float y, float slotStep);
void freeGameBoard(gameBoard* board);

int validMove(gameBoard* board, int col);
void makeMove(gameBoard* board, int col, char player);

state* boardToState(gameBoard* board);
void freeState(state* state);

int evaluate(state* state);

#endif