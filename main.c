#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "drawing.h"

static float windowWidth = 600;
static float windowHeight = 500;

// Poluprecnik kruga svake celije
static float radius = 0.1;

static void on_display(void);
static void on_reshape(int width, int height);
void initialize();

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(200,200);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);

    initialize();

    glutMainLoop();

    return 0;
}

void initialize() {
    glClearColor(1,1,1,0);
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

    glTranslatef(-0.6, 0.2, 0);
    // iscrtava celokupnu tablu za igru, biblioteka "drawing.h"
    drawBoard(0, 0, 0, 0.05, radius);
    
    glutSwapBuffers();
}