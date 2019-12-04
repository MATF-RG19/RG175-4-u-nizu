#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "drawing.h"
#include "gamelib.h"


// poluprecnik kruga svake celije
static const float radius = 0.1;

// horizontalno rastojanje izmedju centara krugova dve susedne kolone
static float slotStep;

// zeton kojim se bira potez
token currToken;
// kolona nad kojom je trenutno currToken, pomera sa tasterima <- i ->
static int currCol = 3;

static float windowWidth = 900;
static float windowHeight = 700;

static void onDisplay(void);
static void onReshape(int width, int height);
static void onKeyboard(unsigned char key, int x, int y);
static void onArrowKey(int key, int x, int y);
void initialize();

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize(windowWidth,windowHeight);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(onDisplay);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(onKeyboard);
    glutSpecialFunc(onArrowKey);

    initialize();

    glutMainLoop();

    return 0;
}

void initialize() {
    glClearColor(0.6,0.6,0.6,0);
    glEnable(GL_DEPTH_TEST);

    slotStep  = 2*radius + radius/4;

    // Podesavaju se pocetne koordinate zetona kojim biramo potez
    currToken.x = 3 * slotStep;
    currToken.y = slotStep;
    currToken.player = 1;
}

static void onReshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)windowWidth/windowHeight, 1, 5);
}

static void onDisplay(void) {

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

    // Crta se zeton kojim se bira potez
    drawToken(&currToken, radius);

    glutSwapBuffers();
}

static void onKeyboard(unsigned char key, int x, int y) {
    (void) x;
    (void) y;

    switch (key) {
        case 27:
            // Zavrsava se program
            exit(EXIT_SUCCESS);
            break;
    }   
}

static void onArrowKey(int key, int x, int y) {
    (void) x;
    (void) y;

    switch (key) {
        case GLUT_KEY_LEFT:
            if(currCol > 0) {
                currToken.x -= slotStep;
                currCol--;
                glutPostRedisplay();
            }
            break;

        case GLUT_KEY_RIGHT:
            if(currCol < 6) {
                currToken.x += slotStep;
                currCol++;
                glutPostRedisplay();
            }
            break;

    }
}