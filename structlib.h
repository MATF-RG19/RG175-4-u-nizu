#ifndef _STRUCTLIB_H_
#define _STRUCTLIB_H_

/**  
 *  Strukture koje se koriste za igru
 */

/**
 *  Podaci o zetonu
 * 
 *  float x, y   - koordinate centra prednjeg i zadnjeg kruga u z-ravni
 *  char player - igrac kojem odgovara zeton
*/
typedef struct token_ {
    float x, y;
    char player;
} token;

/**
 *  Podaci o trenutnoj igri
 * 
 *  token** tokens - matrica zetona
 *  int* topCol    - topCol[j] treba da sadrzi indeks reda prvog slobodnog polja kolone j
 *                   ili -1 ako je kolona j puna do vrha  
*/
typedef struct gameBoard_ {
    token** tokens;
    short* topCol;
} gameBoard;

/**
 *  Stanje igre
 * 
 *  char** st      - matrica zetona, '1' i '2' za 1./2. igraca, '0' kada nema zetona
 *  short* top     - top[j] cuva prvu slobodnu poziciju u j-toj koloni ili -1 ako je nema
 *  short lastMove - koristi je samo minimax da odredi potez koji vodi do tog stanja
*/
typedef struct state_ {
    char** st;
    short* top;
    short lastMove;
} state;

typedef struct minMax_ {
    int value;
    int col;
} minMax;

/**
 *  Koristi je minimax za trazenje novih stanja igre.
 *  
 *  state* a - niz novih stanja
 *  int size - velicina niza
*/
typedef struct stateArr_ {
    state* a;
    int size;
} stateArr;

#endif