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
#define TIMER_ID2 2
#define TIMER_INTERVAL 10

/**
 *  Uputstva za igru:
 *  
 *  levo,desno,dole - kontrole za 1. igraca
 *        JKL       - kontrole za 2. igraca      
 *         R        - resetuje se tabla
 *        ESC       - izlaz iz programa
 *        WASD      - pomeranje kamere
*/

// 2 igraca (1) ili igrac vs. bot (2)
// U opstem slucaju govori da li je igra u toku
static int mode = 0;
// Rezim zavrsene igre, koristi je f-ja za ispis ishoda
static int prevMode = 0;
// 1,2 ili 3 za nereseno, uslovna promenljiva za ispis ishoda
static int winner = 0;
// Bira se tezina igre i po tome se odredjuje dubina za minimax
static int depth = 0;
// Trenutni id igraca
static char player = '1';

// Uslovna promenljiva koja stiti tok programa od odredjenih bagova
static int alreadyReset = 0;
// Uslovna promenljiva za prikazivanje uputstava
static int toggleInstructions = 1;

// Promenljiva za animaciju pada/odskoka zetona
static int animation = 0;

// Promenljiva za animaciju treperenja dobitne kombinacije zetona
static int animation2 = 0;
// Sadrzace koordinate centra 4 dobitna zetona [x1,y1,x2,y2,x3,y3,x4,y4]
float* tokenCoords;
// Animacija se vrsi menjanjem diffuznih koeficijenata materijala zetona.
GLfloat* winnerDiffuseCoeffs;
// U svakom pozivu onTimer2() cini sabirak/umanjilac odgovarajucih koeficijenata.
float coeffStep = 0.01;

// Poluprecnik kruga svake celije
static const float radius = 0.1;
// Horizontalno rastojanje izmedju centara krugova dve susedne kolone
static float slotStep;
// Vektor kretanja za pad zetona
static float vY;

// Zeton kojim se bira potez
token currToken;
// Kolona nad kojom je trenutno currToken, pomera sa tasterima <- i ->
static int currCol = 3;

// Trenutnu tablu igre
gameBoard board;

// Sferne koordinate kamere
static float r = 2;
static float theta = 0;
static float phi = M_PI/3;
// Korak pomeranja kamere
static const float angleStep = M_PI/24;

// Koordinate kamere u xyz-prostoru
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
static void onTimer2(int value);
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
    vY = -slotStep/2;

    // Inicijalizuje se tabla
    board = gameBoardInit(0, 0, slotStep);

    getCameraCoords(r, theta, phi, &eyeX, &eyeY, &eyeZ);

    // Podesavaju se pocetne koordinate zetona kojim biramo potez
    currToken.x = 3 * slotStep;
    currToken.y = slotStep;
    currToken.player = player;

    tokenCoords = malloc(8*sizeof(float));
    if(tokenCoords == NULL) {
        fprintf(stderr, "initialize() malloc fail.\n");
        exit(EXIT_FAILURE);
    }

    winnerDiffuseCoeffs = malloc(4*sizeof(GLfloat));
    if(winnerDiffuseCoeffs == NULL) {
        fprintf(stderr, "initialize() malloc fail.\n");
        exit(EXIT_FAILURE);
    }
    winnerDiffuseCoeffs[0] = winnerDiffuseCoeffs[1] = winnerDiffuseCoeffs[2] = 0.1;
    winnerDiffuseCoeffs[3] = 1;
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

    // Podesava osvetljenje.
    setLightingParams();

    // Iscrtava celokupnu tablu za igru.
    drawBoard(0, 0, 0, 0.05, radius);

    // Crta se zeton kojim se bira potez.
    drawToken(&currToken, radius);
    
    int i,j;
    for(i=0; i<6; i++)
        for(j=0; j<7; j++)
            if(board.tokens[i][j].player != '0')
                drawToken(&board.tokens[i][j], radius);

    // Ako postoji pobednik, iscrtavaju se dobitni zetoni
    if(winner == 1 || winner == 2)
        drawWinningCombo(tokenCoords, radius, winnerDiffuseCoeffs);

    // Ispisuje se prompt za izbor rezima igre (tasteri 1 ili 2)
    if(!mode && !winner)
        printNewGamePrompt(windowWidth, windowHeight);

    // Ispisuje se prompt za izbor tezine igre u 2. rezimu (tasteri 1,2,3)
    if(mode == 2 && !depth)
        printDifficultyPrompt(windowWidth, windowHeight);

    // Ispisuje se uputstva ukoliko je aktiviran prikaz.
    if(toggleInstructions)
        printInstructions(windowWidth, windowHeight);
	
    // Ispisuje se pobednik u slucaju necije pobede i prompt za novu igru/izlaz.
    if(winner)
        printWinner(windowWidth, windowHeight, winner, prevMode);

    glutSwapBuffers();
}

static void onKeyboard(unsigned char key, int x, int y) {
    (void) x;
    (void) y;

    switch (key) {
        case 27:
            // Zavrsava se program.
            freeGameBoard(&board);
            free(tokenCoords);
            free(winnerDiffuseCoeffs);

            exit(EXIT_SUCCESS);
            break;

        // Bira se rezim igre za dva igraca samo kada je ispisan prompt za izbor.
        case '1':
            if(mode == 1 || (mode == 2 && depth))
                break;
            
            if(!mode)
                mode = 1; // kada se bira rezim igre, inace tezina
            else
                depth = 3; // tezina - lako
            
            // Pokrece se igra i omogucava ponovno resetovanje
            alreadyReset = 0;
            
            glutPostRedisplay();
            
            break;

        // Slicno za rezim protiv racunara.
        case '2':
            if(mode == 1 || (mode == 2 && depth))
                break;

            if(!mode)
                mode = 2;
            else
                depth = 5;  // tezina - srednje
            
            alreadyReset = 0;
            glutPostRedisplay();
            
            break;

        // Izbor tezine 3 ako je prethodno izabran 2. rezim
        case '3':
            if(mode != 2 || depth)
                break;

            depth = 8;  // tezina - tesko
            
            alreadyReset = 0;
            glutPostRedisplay();
            
            break;
            
        // Resetuje se igra.
        case 'r':
        case 'R':
            // Izbegava se bespotrebno ponovno resetovanje
            if(alreadyReset)
               break;
            alreadyReset = 1;
            freeGameBoard(&board);
            board = gameBoardInit(0, 0, slotStep);

            // Prekida se pad zetona ako se igra tada resetuje.
            animation = 0;
            // Prekida se treperenje zetona posle pobede.
            animation2 = 0;
            
            mode = 0;
            winner = 0;
            depth = 0;
            
            // Resetuju se vektor i trenutni polozaj slobodnog zetona
            vY = -slotStep/2;
            currToken.x = 3*slotStep;
            currToken.y = slotStep;
            currCol = 3;
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
            if(currCol > 0 && !animation) {
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
            if(currCol < 6 && !animation) {
                currToken.x += slotStep;
                currCol++;
                glutPostRedisplay();
            }
            break;

        // 2.igrac - odigrava se potez ako je validan.
        case 'k':
        case 'K':
            if(mode != 1 || player != '2')
                break;
            if(!animation && validMove(&board, currCol)) {
                
                // Pokrece se animacija pada zetona.
                glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
                animation = 1;
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
            if(currCol > 0 && !animation) {
                currToken.x -= slotStep;
                currCol--;
                glutPostRedisplay();
            }
            break;

        // Zeton se pomera u desno
        case GLUT_KEY_RIGHT:
            if(player != '1')
                break;
            if(currCol < 6 && !animation) {
                currToken.x += slotStep;
                currCol++;
                glutPostRedisplay();
            }
            break;

        // Odigrava se potez ako je validan i ako je igra u toku.
        case GLUT_KEY_DOWN:
            if(!mode || player != '1' || (mode == 2 && !depth))
                break;
            if(!animation && validMove(&board, currCol)) {
                
                // Pokrece se animacija pada zetona.
                glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
                animation = 1;
            }

            break;
    }
}

/**
 *  Animira pad zetona.
 * 
 *  U prvoj fazi (animation = 1) zeton prosto pada. U slucaju najviseg
 *  reda table to je i poslednja faza. 
 * 
 *  Inace se pokrece druga faza (animation = 2) gde zeton blago odskace na gore.
 *  Pocetni vektor odskoka je u startu manjeg intenziteta i sada se u
 *  svakom pozivu dodatno deli ga konstantnim faktorom.
 * 
 *  Kraj druge faze je kada intenzitet stekne vrlo malu vrednost. Menja mu se smer
 *  i pokrece treca faza (animation = 3) i analogno drugoj fazi, intenzitet sada
 *  u svakom pozivu raste dok zeton ne dodje na svoje mesto.
*/
static void onTimer(int value) {
    if(!animation || !mode || value != TIMER_ID)
        return;
    // Uslov za kraj prve faze osim za red table na vrhu
    if(animation == 1 && board.topCol[currCol] > 0 &&
        currToken.y <= board.tokens[board.topCol[currCol]][currCol].y) {
            
        animation = 2;
        vY = -vY/3;
        currToken.y += vY;
        
        glutPostRedisplay();
        glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
        
        return;
    }
    // Uslov za kraj druge faze
    if(animation == 2 && vY <= 0.00001) {
        
        animation = 3;
        vY *= -1;
    }
    // Obuhvata slucaj kraja trece faze i slucaja kada zeton ide u najvisi red
    if(animation != 2 && 
        currToken.y <= board.tokens[board.topCol[currCol]][currCol].y) {
        
        animation = 0;
        vY = -slotStep/2;
        
        // Kada padne zeton azurira se tabla.
        makeMove(&board, currCol, player);
        
        state* state = boardToState(&board);
        int score = evaluate(state);

        if(score == -1000) {
            // Pobedio je 2.igrac / racunar.
            prevMode = mode;
            mode = 0;
            winner = 2;
            
            // Traze se 4 dobitna zetona, premestaju se iz glavne table da bi se posebno crtali
            getWinningCombo(&board, tokenCoords);
            // Pocetni difuzni koeficijenti u animaciji ako su zetoni zutii.
            winnerDiffuseCoeffs[0] = 0.5;
            winnerDiffuseCoeffs[1] = 0.5;

            glutTimerFunc(TIMER_INTERVAL, onTimer2, TIMER_ID2);
            animation2 = 1;
            
            glutPostRedisplay();
        } else {
            int i, topSum = 7;
            for(i=0; i<7; i++)
                topSum += state->top[i];
            // Ako su sve kolone pune, top[i] je svuda -7 a topSum = 0
            if(!topSum) {
                mode = 0;
                winner = 3;
                glutPostRedisplay();
            }
        }
        
        // Id igraca na potezu se alternira.
        player = player == '1' ? '2' : '1';
        currToken.player = player;
        currToken.y = slotStep;

        glutPostRedisplay();
        
        if(score == 1000) {
            prevMode = mode;
            mode = 0;
            winner = 1;

            // Traze se 4 dobitna zetona, premestaju se iz glavne table da bi se posebno crtali
            getWinningCombo(&board, tokenCoords);
            // Pocetni difuzni koeficijenti u animaciji ako su zetoni crveni.
            winnerDiffuseCoeffs[0] = 0.5;
            winnerDiffuseCoeffs[1] = 0.1;
            
            glutTimerFunc(TIMER_INTERVAL, onTimer2, TIMER_ID2);
            animation2 = 1;

            glutPostRedisplay();
        }
        
        // Ako se igra protiv racunara, sada je na njega red.
        if(mode == 2 && player == '2') {
            /* 
                Depth = 8 je najveca dubina na kojoj se botMakeMove()
                "odmah" izvrsava, osim u eventualno 2,3 poteza na pocetku igre
                kada laguje za ~1s
            */
            int botCol = botMakeMove(state, depth);
            
            // Racunar zeton se iscrtava nad izabranom kolonom.
            currToken.x += (botCol - currCol)*slotStep;
            currCol = botCol;
            glutPostRedisplay();
            
            /*
                Pokrece se animacija za pad racunarovog zetona.
                Interval je malo "veci" jer se u skoro svim slucajevima
                minimax brzo zaustavlja i zuti zeton pada odmah za crvenim
                sto nije optimalno/realisticno.
            */
            glutTimerFunc(300, onTimer, TIMER_ID);
            animation = 1;
        }
        
        freeState(state);

        return;
    }
    if(animation == 2)
        vY /= 1.5;
    else if(animation == 3 && vY > -slotStep/16)
        vY *= 1.5;
    
    currToken.y += vY;
    glutPostRedisplay();

    if (animation) {
        glutTimerFunc(TIMER_INTERVAL, onTimer, TIMER_ID);
    }
}

/**
 *  Pokrece se u slucaju necije pobede, dobitna kombinacija treperi.
*/
static void onTimer2(int value) {
    if(value != TIMER_ID2)
        return;

    // Menja se znak na granicama.
    if(winnerDiffuseCoeffs[0] < 0.5 || winnerDiffuseCoeffs[0] > 1)
        coeffStep *= -1;
    
    // Menjaju se difuzni koeficijenti materijala zetona.
    winnerDiffuseCoeffs[0] += coeffStep;
    if(winner == 2)
        winnerDiffuseCoeffs[1] += coeffStep;

    glutPostRedisplay();

    if (animation2) {
        glutTimerFunc(TIMER_INTERVAL, onTimer2, TIMER_ID2);
    }
}