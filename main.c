#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "drawing.h"
#include "gamelib.h"
#include "structlib.h"

#define TIMER_ID 1
#define TIMER_INTERVAL 10 

/**
 *  Uputstva za igru:
 *  
 *  (TODO: na "ekran" dodati uputstva za igru i potrebna obavestenja)
 *  
 *  levo,desno,dole - kontrole za 1. igraca
 *        JKL       - kontrole za 2. igraca      
 *         R        - resetuje se tabla
 *        ESC       - izlaz iz programa
 *        WASD      - pomeranje kamere
 * 
 *  Napomena: 
 *      Trenutno se mod igre eksplicitno podesava u funkciji initialize() (mode=1 ili mode=2),
 *  prvi rezim za dva igraca i drugi za igraca protiv racunara.
*/

// 2 igraca (1) ili igrac vs. bot (2)
static int mode;

// 1 ako je igra u toku, 0 inace
static int game;

static int animationOngoing = 0;

// Uslovna promenljiva za prikazivanje uputstava
static int toggleInstructions = 1;

// poluprecnik kruga svake celije
static const float radius = 0.1;

// horizontalno rastojanje izmedju centara krugova dve susedne kolone
static float slotStep;
// vektor kretanja za pad zetona
static float vY;

// zeton kojim se bira potez
token currToken;
// kolona nad kojom je trenutno currToken, pomera sa tasterima <- i ->
static int currCol = 3;

// pokazivac na trenutnu tablu igre
gameBoard board;
// trenutni id igraca
static char player = '1';

// sferne koordinate kamere
static float r = 2;
static float theta = 0;
static float phi = M_PI/2;
// korak pomeranja kamere
static const float angleStep = M_PI/24;

// koordinate kamere u xyz-prostoru
static float eyeX;
static float eyeY;
static float eyeZ;

static float windowWidth = 900;
static float windowHeight = 700;

static void onDisplay(void);
static void onReshape(int width, int height);
static void onKeyboard(unsigned char key, int x, int y);
static void onArrowKey(int key, int x, int y);
static void onTimer(int value);
void initialize();

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, windowWidth, windowHeight, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
void renderSpacedBitmapString(

			float x,
			float y,
			int spacing,
			void *font,
			char *string) {

  char *c;
  int x1=x;

  for (c=string; *c != '\0'; c++) {

	glRasterPos2f(x1,y);
	glutBitmapCharacter(font, *c);
	x1 = x1 + glutBitmapWidth(font,*c) + spacing;
  }
}
void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
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
    vY = -slotStep/2;

    // Inicijalizuje se tabla
    board = gameBoardInit(0, 0, slotStep);

    mode = 2;
    game = 1;

    getCameraCoords(r, theta, phi, &eyeX, &eyeY, &eyeZ);

    // Podesavaju se pocetne koordinate zetona kojim biramo potez
    currToken.x = 3 * slotStep;
    currToken.y = slotStep;
    currToken.player = player;
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, windowWidth/(float)windowHeight, 1, 5);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, 0, 1, 0);

    glTranslatef(-0.6, 0.2, 0);

    // Podesava osvetljenje
    setLightingParams();

    // Iscrtava celokupnu tablu za igru
    drawBoard(0, 0, 0, 0.05, radius);

    // Crta se zeton kojim se bira potez
    drawToken(&currToken, radius);
    
    int i,j;
    for(i=0; i<6; i++)
        for(j=0; j<7; j++)
            if(board.tokens[i][j].player != '0')
                drawToken(&board.tokens[i][j], radius);

    // Ispisuje se uputstva ukoliko je aktiviran prikaz
    if(toggleInstructions)
        printInstructions(windowWidth, windowHeight);
	
    glutSwapBuffers();
}

static void onKeyboard(unsigned char key, int x, int y) {
    (void) x;
    (void) y;

    switch (key) {
        case 27:
            // Zavrsava se program.
            freeGameBoard(&board);
            exit(EXIT_SUCCESS);
            break;

        // Resetuje se tabla.
        case 'r':
        case 'R':
            freeGameBoard(&board);
            board = gameBoardInit(0, 0, slotStep);
            game = 1;
            currToken.player = player = '1';

            glutPostRedisplay();

            break;

        // Prikazuju se ili sakrivaju uputstva.
        case 'v':
        case 'V':
            toggleInstructions = !toggleInstructions;
            glutPostRedisplay();
            break;

        // Pomera se kamera.
        case 'a':
        case 'A':
            if(theta > -M_PI/3) {
                theta -= angleStep;
                getCameraCoords(r, theta, phi, &eyeX, &eyeY, &eyeZ);
                glutPostRedisplay();
            }
            break;

        case 'd':
        case 'D':
            if(theta < M_PI/3) {
                theta += angleStep;
                getCameraCoords(r, theta, phi, &eyeX, &eyeY, &eyeZ);
                glutPostRedisplay();
            }
            break;

        case 'w':
        case 'W':
            if(phi > M_PI/6) {
                phi -= angleStep;
                getCameraCoords(r, theta, phi, &eyeX, &eyeY, &eyeZ);
                glutPostRedisplay();
            }
            break;

        case 's':
        case 'S':
            if(phi < M_PI/2) {
                if (phi > M_PI/2) 
                    phi = M_PI/2;
                phi += angleStep;
                getCameraCoords(r, theta, phi, &eyeX, &eyeY, &eyeZ);
                glutPostRedisplay();
            }
            break;
        
        // 2. igrac - zeton nad tablom se pomera za jedno mesto u levo
        case 'j':
        case 'J':
            if(mode != 1 || player != '2')
                break;
            if(currCol > 0 && !animationOngoing) {
                currToken.x -= slotStep;
                currCol--;
                glutPostRedisplay();
            }
            break;

        // 2. igrac - zeton se pomera u desno
        case 'l':
        case 'L':
            if(mode != 1 || player != '2')
                break;
            if(currCol < 6 && !animationOngoing) {
                currToken.x += slotStep;
                currCol++;
                glutPostRedisplay();
            }
            break;

        // 2.igrac - odigrava se potez ako je validan.
        case 'k':
        case 'K':
            if(!game || mode != 1 || player != '2')
                break;
            if(!animationOngoing && validMove(&board, currCol)) {
                
                // Pokrece se animacija pada zetona.
                glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
                animationOngoing = 1;
            }
            break;
    }   
}

static void onArrowKey(int key, int x, int y) {
    (void) x;
    (void) y;

    switch (key) {
        // Zeton nad tablom se pomera za jedno mesto u levo
        case GLUT_KEY_LEFT:
            if(player != '1')
                break;
            if(currCol > 0 && !animationOngoing) {
                currToken.x -= slotStep;
                currCol--;
                glutPostRedisplay();
            }
            break;

        // Zeton se pomera u desno
        case GLUT_KEY_RIGHT:
            if(player != '1')
                break;
            if(currCol < 6 && !animationOngoing) {
                currToken.x += slotStep;
                currCol++;
                glutPostRedisplay();
            }
            break;

        // Odigrava se potez ako je validan.
        case GLUT_KEY_DOWN:
            if(!game || player != '1')
                break;
            if(!animationOngoing && validMove(&board, currCol)) {
                
                // Pokrece se animacija pada zetona.
                glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
                animationOngoing = 1;
            }

            break;
    }
}

static void onTimer(int value) {
    if(!animationOngoing || !game || value != TIMER_ID)
        return;

    if(currToken.y <= board.tokens[board.topCol[currCol]][currCol].y) {
        animationOngoing = 0;
        
        // Kada padne zeton azurira se tabla.
        makeMove(&board, currCol, player);
        
        state* state = boardToState(&board);
        int score = evaluate(state);

        if(score == -1000) {
            game = 0;
            printf("Pobednik: %d.\n", score == 1000 ? 1 : 2);
        }
        
        // Id igraca na potezu se alternira.
        player = player == '1' ? '2' : '1';
        currToken.player = player;
        currToken.y = slotStep;

        glutPostRedisplay();
        
        if(score == 1000) {
            game = 0;
            printf("Pobednik: 1.\n");
        }
        
        // Ako se igra protiv racunara, sada je na njega red.
        if(game && mode == 2 && player == '2') {
            int botCol = botMakeMove(state, 8);
            
            // Racunar zeton se iscrtava nad izabranom kolonom.
            currToken.x += (botCol - currCol)*slotStep;
            currCol = botCol;
            glutPostRedisplay();
            
            // Pokrece se animacija za pad zetona.
            glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
            animationOngoing = 1;
        }
        
        freeState(state);

        return;
    }

    currToken.y += vY;
    glutPostRedisplay();

    if (animationOngoing) {
        glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
    }
}
