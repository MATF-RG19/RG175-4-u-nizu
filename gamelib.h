#ifndef _GAMELIB_H_
#define _GAMELIB_H

#include "structlib.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
minMax minimax(state* startState, int depth, char player, int alpha, int beta);

state* copyState(state* src);

#endif