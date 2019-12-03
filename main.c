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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
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

    // podesavanja za svetlo
    GLfloat light_position[] = { 0, 0.2, 1, 0 };
    GLfloat light_ambient[] = { 0, 0, 0, 1 };
    GLfloat light_diffuse[] = { 1, 1, 1, 1 };
    GLfloat light_specular[] = { 0.5, 0.5, 0.5, 1 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    glTranslatef(-0.6, 0.2, 0);
    // iscrtava celokupnu tablu za igru, biblioteka "drawing.h"
    drawBoard(0, 0, 0, 0.05, radius);
    

    glutSwapBuffers();
}