#include "drawing.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

/**
 *  Definicije funkcija koje iscrtavaju elemente za igru
**/


/** 
 *  Iscrtava celu tablu za igru.
 * 
 *  x,y - koordinate centra kruga gornje leve celije.
 *  z,w - z koordinata prednje table, z-w zadnje
 *  radius - poluprecnik kruga svake celije
 * 
 *  Napomena -- trenutno nije podeseno osvetljenje ni materijal,
 *      koriste se placeholder boje.
**/
void drawBoard(float x, float y, float z, float w, float radius) {
    glColor3f(0.5,0.5,0);
    // crtaju se prednja i zadnja ploca
    drawTable(x, y, z, radius);
    glColor3f(0,0.5,0.5);
    drawTable(x, y, z-w, radius);

    // Spajanje ploca unutrasnjim pregradama i spoljasnjim ivicama
    float border = radius/4;
    float startX = x - radius - border;
    float endX = startX + 14*radius + 8*border;
    float startY = y + radius + border;
    float endY = startY - 12*radius - 7*border;
    glColor3f(0,0,0.75);
    
    glBegin(GL_POLYGON);
        glVertex3f(startX, endY, 0);
        glVertex3f(startX, endY, -w);
        glVertex3f(endX, endY, -w);
        glVertex3f(endX, endY, 0);
    glEnd();
    
    glBegin(GL_POLYGON);
            glVertex3f(startX, startY, 0);
            glVertex3f(startX, startY, -w);
            glVertex3f(startX, endY, -w);
            glVertex3f(startX, endY, 0);
    glEnd();

    startX += 2*radius + 3*border/2;
    int i;
    for(i=0; i<6; i++, startX += 2*radius + border) {
        glBegin(GL_POLYGON);
            glVertex3f(startX, startY, 0);
            glVertex3f(startX, startY, -w);
            glVertex3f(startX, endY, -w);
            glVertex3f(startX, endY, 0);
        glEnd();
    }
    
    startX += border/2;
    glBegin(GL_POLYGON);
            glVertex3f(startX, startY, 0);
            glVertex3f(startX, startY, -w);
            glVertex3f(startX, endY, -w);
            glVertex3f(startX, endY, 0);
    glEnd();
}
/** 
 *  Pomocna funkcija, poziva je drawBoard().
 * 
 *  Iscrtava 7x6 plocu sa kruznim supljinama.
**/
void drawTable(float x, float y, float z, float radius) {
    int i,j;

    // Crtanje granica svih celija

    float tmpX, tmpY = y;
    for(i=0; i<6; i++) {
        tmpX = x;
        for(j=0; j<7; j++) {
            // Crtanje granica za jednu celiju koje zajedno sa njom cine kvadrat    
            drawCorners(tmpX,tmpY, z,radius);
            tmpX += 2*radius + radius/4;
        }
        tmpY -= (2*radius + radius/4);
    }
    
    // Crtanje granica na ivicama table
    
    float width = radius/4;
    float startX = -radius + x, endX = startX + 14*radius + 6*width;
    float startY = radius + y, endY = startY - 12*radius - 5*width;
    
    // Crtanje horizontalnih i vertikalnih granica izmedju celija

    startX = x - radius - width, endX = startX + width;
    startY = radius + y, endY = startY - 12*radius - 5*width;
    for(i=0; i<8; i++) {
        glBegin(GL_POLYGON);
            glVertex3f(startX, startY, z);
            glVertex3f(endX, startY, z);
            glVertex3f(endX, endY, z);
            glVertex3f(startX, endY, z);
        glEnd();
        startX += 2*radius + width;
        endX += 2*radius + width;
    }
    
    startX = x - radius - width, endX = startX + 14*radius + 8*width;
    startY = y + radius + width, endY = startY - width;
    for(i=0; i<7; i++) {
        glBegin(GL_POLYGON);
            glVertex3f(startX, startY, z);
            glVertex3f(endX, startY, z);
            glVertex3f(endX, endY, z);
            glVertex3f(startX, endY, z);
        glEnd();
        startY -= (2*radius + width);
        endY -= (2*radius + width);
    } 
}
/** 
 *  Pomocna funkcija, poziva je drawTable().
 * 
 *  Krug sa poluprecnikom radius i centrom u (x,y) je upisan u kvadrat
 *  sa stranicom 2*radius. Svaki krug je supljina na ploci pa se ne ispunjava
 *  Ova funkcija iscrtava komplement jednom takvom kvadratu u ravni z.  
**/
void drawCorners(float x, float y, float z, float radius) {
    int i;
    // ugao za iteraciju
    float baseAngle = M_PI/12;
    float angle = 0;

    // Iscrtavaju se 4 coska kvadrata ogranicena upisanim krugom
    glBegin(GL_POLYGON);
        glVertex3f(radius + x, radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle)
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
    glEnd();
    
    angle -= baseAngle;
    glBegin(GL_POLYGON);
        glVertex3f(-radius + x, radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle)
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
    glEnd();
    
    angle -= baseAngle;
    glBegin(GL_POLYGON);
        glVertex3f(-radius + x, -radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle)
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
    glEnd();

    angle -= baseAngle;
    glBegin(GL_POLYGON);
        glVertex3f(radius + x, -radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle)
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
    glEnd();
}

