#ifndef _STRUCTLIB_H_
#define _STRUCTLIB_H_

/**  
 *  Strukture koje se koriste za igru
 */

/**
 *  Podaci o zetonu
 * 
 *  float x, y   - koordinate centra prednjeg i zadnjeg kruga u z-ravni
 *  int player - igrac kojem odgovara zeton
*/
typedef struct token_ {
    float x, y;
    int player;
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
    int* topCol;
} gameBoard;

#endif