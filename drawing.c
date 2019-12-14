#include "drawing.h"
#include "structlib.h"

/**
 *  Funkcije za iscrtavanje elemenata za igru, osvetljenje
*/


/**
 *  Podesava osvetljenje 
 */
void setLightingParams() {
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
}

/** 
 *  Iscrtava celu tablu za igru.
 * 
 *  x,y - koordinate centra kruga gornje leve celije.
 *  z,w - z koordinata prednje table, z-w zadnje
 *  radius - poluprecnik kruga svake celije
*/
void drawBoard(float x, float y, float z, float w, float radius) {
    
    // podesavanja za materijal
    // zadnja ploca ce biti tamnija
    GLfloat ambientCoeffs[] = {0, 0, 0, 1 };
    GLfloat diffuseCoeffs[] = { 0, 0, 0.2, 1 };
    GLfloat specularCoeffs[] = { 0, 0, 0, 1 };
    GLfloat shininess = 0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCoeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularCoeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    drawTable(x, y, z-w, radius);

    // ostali delovi su svetliji
    // GLfloat diffuse_coeffs2[] = { 0.1, 0.1, 0.4, 1 };
    diffuseCoeffs[2] = 0.4;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCoeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularCoeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // crta se prednja ploca
    drawTable(x, y, z, radius);

    diffuseCoeffs[2] = 0.3;
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCoeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularCoeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // Spajanje ploca unutrasnjim pregradama i spoljasnjim ivicama
    float border = radius/4;
    float startX = x - radius - border;
    float endX = startX + 14*radius + 8*border;
    float startY = y + radius + border;
    float endY = startY - 12*radius - 7*border;
    
    glBegin(GL_POLYGON);
        glNormal3f(startX, endY, 1);
        glVertex3f(startX, endY, 0);
        glNormal3f(startX, endY, 1);
        glVertex3f(startX, endY, -w);
        glNormal3f(endX, endY, 1);
        glVertex3f(endX, endY, -w);
        glNormal3f(endX, endY, 1);
        glVertex3f(endX, endY, 0);
    glEnd();
    
    glBegin(GL_POLYGON);
        glNormal3f(startX, startY, 1);
        glVertex3f(startX, startY, 0);
        glNormal3f(startX, startY, 1);
        glVertex3f(startX, startY, -w);
        glNormal3f(startX, endY, 1);
        glVertex3f(startX, endY, -w);
        glNormal3f(startX, endY, 1);
        glVertex3f(startX, endY, 0);
    glEnd();

    startX += 2*radius + 3*border/2;
    int i;
    for(i=0; i<6; i++, startX += 2*radius + border) {
        glBegin(GL_POLYGON);
            glNormal3f(startX, startY, 1);
            glVertex3f(startX, startY, 0);
            glNormal3f(startX, startY, 1);
            glVertex3f(startX, startY, -w);
            glNormal3f(startX, endY, 1);
            glVertex3f(startX, endY, -w);
            glNormal3f(startX, endY, 1);
            glVertex3f(startX, endY, 0);
        glEnd();
    }
    
    startX += border/2;
    glBegin(GL_POLYGON);
        glNormal3f(startX, startY, 1);
        glVertex3f(startX, startY, 0);
        glNormal3f(startX, startY, 1);
        glVertex3f(startX, startY, -w);
        glNormal3f(startX, endY, 1);
        glVertex3f(startX, endY, -w);
        glNormal3f(startX, endY, 1);
        glVertex3f(startX, endY, 0);
    glEnd();
}
/** 
 *  Pomocna funkcija, poziva je drawBoard().
 * 
 *  Iscrtava 7x6 plocu sa kruznim supljinama.
*/
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
            glNormal3f(startX, startY, 1);
            glVertex3f(startX, startY, z);
            glNormal3f(endX, startY, 1);
            glVertex3f(endX, startY, z);
            glNormal3f(endX, endY, 1);
            glVertex3f(endX, endY, z);
            glNormal3f(startX, endY, 1);
            glVertex3f(startX, endY, z);
        glEnd();
        startX += 2*radius + width;
        endX += 2*radius + width;
    }
    
    startX = x - radius - width, endX = startX + 14*radius + 8*width;
    startY = y + radius + width, endY = startY - width;
    for(i=0; i<7; i++) {
        glBegin(GL_POLYGON);
            glNormal3f(startX, startY, 1);
            glVertex3f(startX, startY, z);
            glNormal3f(endX, startY, 1);
            glVertex3f(endX, startY, z);
            glNormal3f(endX, endY, 1);
            glVertex3f(endX, endY, z);
            glNormal3f(startX, endY, 1);
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
*/
void drawCorners(float x, float y, float z, float radius) {
    int i;
    // ugao za iteraciju
    float baseAngle = M_PI/12;
    float angle = 0;

    // Iscrtavaju se 4 coska kvadrata ogranicena upisanim krugom
    glBegin(GL_POLYGON);
        glNormal3f(radius + x, radius + y, 1);
        glVertex3f(radius + x, radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle) {
            glNormal3f(cos(angle)*radius + x, sin(angle)*radius + y, 1);
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
        }
    glEnd();
    
    angle -= baseAngle;
    glBegin(GL_POLYGON);
        glNormal3f(-radius + x, radius + y, 1);
        glVertex3f(-radius + x, radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle) {
            glNormal3f(cos(angle)*radius + x, sin(angle)*radius + y, 1);
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
        }
    glEnd();
    
    angle -= baseAngle;
    glBegin(GL_POLYGON);
        glNormal3f(-radius + x, -radius + y, 1);
        glVertex3f(-radius + x, -radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle) {
            glNormal3f(cos(angle)*radius + x, sin(angle)*radius + y, 1);
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
        }
    glEnd();

    angle -= baseAngle;
    glBegin(GL_POLYGON);
        glNormal3f(radius + x, -radius + y, 1);
        glVertex3f(radius + x, -radius + y, z);
        for(i=0; i<7; i++, angle += baseAngle) {
            glNormal3f(cos(angle)*radius + x, sin(angle)*radius + y, 1);
            glVertex3f(cos(angle)*radius + x, sin(angle)*radius + y, z);
        }
    glEnd();
}
/** 
 *  Iscrtava zeton odgovarajuce boje
 *  
 *  t      - pokazivac na token
 *  radius - poluprecnik kruga
*/
void drawToken(token* t , float radius) {

    // podesavanja za materijal
    // prednji i zadnji krug
    GLfloat ambientCoeffs[] = {0, 0, 0, 1 };
    GLfloat diffuseCoeffs[] = { 0.5, 0.1, 0.1, 1 }; // crvena za player = '1'
    if(t->player == '2')
        diffuseCoeffs[1] = 0.5; // zuta za player = '2'

    GLfloat specularCoeffs[] = { 0, 0, 0, 1 };
    GLfloat shininess = 0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCoeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularCoeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // ugao za iteraciju
    float angle = M_PI/12;
    
    const float x = t->x;
    const float y = t->y;
    const float z = -0.01; // z-koordinata prednjeg kruga
    const float w =  0.03;  // debljina zetona 

    float tmpAngle;

    // prednji krug
    glBegin(GL_POLYGON);
        for(tmpAngle=0; tmpAngle<2*M_PI; tmpAngle += angle) {
            glNormal3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, 1);
            glVertex3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, z);
        }
    glEnd();
    
    // zadnji krug
    glBegin(GL_POLYGON);
        for(tmpAngle=0; tmpAngle<2*M_PI; tmpAngle += angle) {
            glNormal3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, 1);
            glVertex3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, z - w);
        }
    glEnd();

    // ivice zetona su tamnije
    GLfloat diffuse_coeffs2[] = { 0.3, 0, 0, 1 }; // crvena za player = '1'
    if(t->player == '2')
       diffuse_coeffs2[1] = 0.3; // zuta za player = '2'
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularCoeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // crta se ivica zetona
    float tmpZ = z;
    int i;
    glBegin(GL_TRIANGLE_STRIP);
        for(i=0; i<26; i++,tmpAngle += angle) {
            glNormal3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, 1);
            glVertex3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, tmpZ);
            if(tmpZ == z)
                tmpZ -= w;
            else
                tmpZ += w;
        }
    glEnd();
}

/**
 *  Poziva se u slucaju necije pobede.
 * 
 *  Varijacija koda iz funkcije drawToken(), s tim da odjednom crta 4 zetona
 *  cijih su koordinate u nizu tokens i tokom animacije treperenja se menjaju
 *  diffuseCoeffs.
*/
void drawWinningCombo(float* tokens, float radius, GLfloat* diffuseCoeffs) {
    GLfloat ambientCoeffs[] = {0, 0, 0, 1 };
    GLfloat specularCoeffs[] = { 0, 0, 0, 1 };
    GLfloat shininess = 0;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCoeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCoeffs);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularCoeffs);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    
    // ugao za iteraciju
    float angle = M_PI/12;
    int i;
    for(i=0; i<8; i+=2) {
        const float x = tokens[i]; // x-koordinata centra prednjeg i zadnjeg kruga
        const float y = tokens[i+1]; // y-koordinata -||-
        const float z = -0.01; // z-koordinata prednjeg kruga
        const float w =  0.03;  // debljina zetona 

        float tmpAngle;

        // prednji krug
        glBegin(GL_POLYGON);
            for(tmpAngle=0; tmpAngle<2*M_PI; tmpAngle += angle) {
                glNormal3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, 1);
            glVertex3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, z);
            }
        glEnd();
    
        // zadnji krug
        glBegin(GL_POLYGON);
            for(tmpAngle=0; tmpAngle<2*M_PI; tmpAngle += angle) {
                glNormal3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, 1);
                glVertex3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, z - w);
            }
        glEnd();
    
        // crta se ivica zetona
        float tmpZ = z;
        int i;
        glBegin(GL_TRIANGLE_STRIP);
            for(i=0; i<26; i++,tmpAngle += angle) {
                glNormal3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, 1);
                glVertex3f(cos(tmpAngle)*radius + x, sin(tmpAngle)*radius + y, tmpZ);
                if(tmpZ == z)
                    tmpZ -= w;
                else
                    tmpZ += w;
            }
        glEnd();
    }
}

/**
 *  Pretvara (r,theta,phi) koordinate kamere i smesta u promenljive x,y,z
 * 
 *  float r      - rastojanje od table
 *  float theta  - ugao izmedju x i z osa
 *  float z      - ugao izmedju z-ose i vektora odredjenim uz pomoc theta
 *  float* x,y,z - lokacije na kojima se smestaju nove koordinate
*/
void getCameraCoords(float r, float theta, float phi, float* x, float* y, float* z) {
    *x = r * sin(theta) * sin(phi);
    *y = r * cos(phi); 
    *z = r * cos(theta) * sin(phi);
}

/**
 *  Ispisuje uputstva za igru u gornjem levom uglu.
*/
void printInstructions(float windowWidth, float windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(0,0,0.3);

    glRasterPos2f(windowWidth/20,windowHeight/15);
    char* c = "Kontrole:";

    int i;
    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[i]);

    glRasterPos2f(windowWidth/20,1.5*windowHeight/15);
    c = "Igrac 1: <-V->";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c[i]);

    glRasterPos2f(windowWidth/20,2*windowHeight/15);
    c = "Igrac 2: JKL";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c[i]);

    glRasterPos2f(windowWidth/20,3*windowHeight/15);
    c = "Kamera: WASD";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c[i]);

    glRasterPos2f(windowWidth/20,3.5*windowHeight/15);
    c = "Reset: R";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c[i]);

    glRasterPos2f(windowWidth/20,4*windowHeight/15);
    c = "(sakrij: V)";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c[i]);
}

/**
 *  Ispisuje pobednika i prompt za novu igru/izlaz
*/
void printWinner(float windowWidth, float windowHeight, int winner, int mode) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    if(winner == 1)
        glColor3f(0.6,0,0);
    else if(winner == 2)
        glColor3f(0.5,0.5,0);
    else
        glColor3f(0,0,0.5);

    char* c;
    if(winner != 3) {
        if(mode == 1) {
            if(winner == 1)
                c = "Pobednik: 1. IGRAC!";
            else
                c = "Pobednik: 2. IGRAC!";
        } else {
            if(winner == 1)
                c = "Pobednik: IGRAC!";
            else
                c = "Pobednik: RACUNAR!";
        }
        glRasterPos2f(2.1*windowWidth/5,2*windowHeight/15);
    } else {
        c = "NERESENO!";
        glRasterPos2f(2.31*windowWidth/5,2*windowHeight/15);
    }

    int i;
    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[i]);

    glColor3f(0,0.5,0);
    glRasterPos2f(2.31*windowWidth/5,2.6*windowHeight/15);
    c = "nova igra - R";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c[i]);

    glColor3f(0,0,0);
    glRasterPos2f(2.37*windowWidth/5,2.9*windowHeight/15);
    c = "izlaz - ESC";

    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c[i]);
}

/**
 *  Ispisuje prompt za izbor rezima igre.
*/
void printNewGamePrompt(float windowWidth, float windowHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    char* c = "IZABERITE REZIM IGRE";
    glColor3f(0,0,0.7);
    glRasterPos2f(2.12*windowWidth/5,2*windowHeight/15);

    int i;
    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[i]);

    glColor3f(0,0,0.5);
    c = "(1) Igrac 1 - Igrac 2";
    glRasterPos2f(2.15*windowWidth/5,2.5*windowHeight/15);
    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c[i]);

    c = "(2) Igrac - Racunar ";
    glRasterPos2f(2.2*windowWidth/5,2.8*windowHeight/15);
    for(i=0; c[i]; i++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c[i]);
}