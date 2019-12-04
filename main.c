#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "drawing.h"

static float windowWidth = 900;
static float windowHeight = 700;

// poluprecnik kruga svake celije
static float radius = 0.1;

static void on_display(void);
static void on_reshape(int width, int height);
void initialize();

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);

    initialize();

    glutMainLoop();

    return 0;
}

void initialize() {
    glClearColor(0.6,0.6,0.6,0);
    glEnable(GL_DEPTH_TEST);
}

static void on_reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)windowWidth/windowHeight, 1, 5);
}

static void on_display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, windowWidth/(float)windowHeight, 1, 5);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 1, 1.6, 0, 0, 0, 0, 1, 0);

    // Podesava osvetljenje
    setLightingParams();
    
    glTranslatef(-0.6, 0.2, 0);
    // Iscrtava celokupnu tablu za igru
    drawBoard(0, 0, 0, 0.05, radius);

    // Za sada samo crta jedan zeton nad prvom kolonom
    drawToken(0, 2*radius + radius/2, -0.01, 0.03, radius, 1);

    glutSwapBuffers();
}