#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/*
 *  Biblioteka za iscrtavanje elemenata za igru, osvetljenje
*/

void drawTable(float x, float y, float z, float radius);
void drawCorners(float x, float y, float z, float radius);
void drawBoard(float x, float y, float z, float w, float radius);

void drawToken(float x, float y, float z, float w, float radius, int player);

void setLightingParams();

#endif