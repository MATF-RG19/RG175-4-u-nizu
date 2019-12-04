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

void setLightingParams();

#endif