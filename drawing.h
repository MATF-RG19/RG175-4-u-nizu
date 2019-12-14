#ifndef _DRAWING_H_
#define _DRAWING_H_

#include "structlib.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/**
 *  Biblioteka za iscrtavanje elemenata za igru, osvetljenje i slicno
*/

void drawTable(float x, float y, float z, float radius);
void drawCorners(float x, float y, float z, float radius);
void drawBoard(float x, float y, float z, float w, float radius);

void drawToken(token* t, float radius);
void drawWinningCombo(float* tokens, float radius, GLfloat* diffuseCoeffs);

void setLightingParams();

void getCameraCoords(float r, float theta, float phi, float* x, float* y, float* z);

void printInstructions(float windowWidth, float windowHeight);
void printWinner(float windowWidth, float windowHeight, int winner, int mode);
void printNewGamePrompt(float windowWidth, float windowHeight);

#endif