#ifndef _GAMELIB_H_
#define _GAMELIB_H

#include "structlib.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 *  Biblioteka za logiku igre
 * 
 *  Detaljnije u gamelib.c
*/

gameBoard gameBoardInit(float x, float y, float slotStep);
void freeGameBoard(gameBoard* board);

int validMove(gameBoard* board, int col);
void makeMove(gameBoard* board, int col, char player);

state* boardToState(gameBoard* board);
void freeState(state* state);

int evaluate(state* state);
void getWinningCombo(gameBoard* board, float* tokens);
minMax minimax(state* startState, int depth, char player, int alpha, int beta);

int botMakeMove(state* state, int depth);

state* copyState(state* src);

#endif
