#ifndef _STRUCTLIB_H_
#define _STRUCTLIB_H_

/**  
 *  Strukture koje se koriste za igru
 */

/**
 *  Podaci o zetonu
 * 
 *  x, y   - koordinate centra prednjeg i zadnjeg kruga u z-ravni
 *  player - igrac kojem odgovara zeton
*/
typedef struct token_ {
    float x, y;
    int player;
} token;


#endif