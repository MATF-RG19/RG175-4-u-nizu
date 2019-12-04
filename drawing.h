#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/**
 *  Biblioteka za iscrtavanje elemenata za igru, osvetljenje i slicno
*/

/**
 *  Podaci o zetonu
 * 
 *  x, y   - koordinate centra prednjeg i zadnjeg kruga u z-ravni
 *  player - igrac kojem odgovara zeton
*/
typedef struct token_ {
    float x, y;
    int player;
} token;

void drawTable(float x, float y, float z, float radius);
void drawCorners(float x, float y, float z, float radius);
void drawBoard(float x, float y, float z, float w, float radius);

void drawToken(token* t, float radius);

void setLightingParams();

#endif