#include "gamelib.h"
#include "structlib.h"

/**
 *  Funkcije za logiku igre
*/

/**
 *  Proverava da li je izbor kolone validan potez
 * 
 *  gameBoard* board - pokazivac na tablu
 *  int col          - izabrana kolona, validan opseg [0,6]
 * 
 *  Vraca 1 ako je izbor validan, inace 0;
*/ 
int validMove(gameBoard* board, int col) {
    if(col<0 || col>6 || board->topCol[col] == -1)
        return 0;
    return 1;
}

/** 
 *  Odigrava potez u odabranoj koloni ukoliko je to moguce.
 *  Podrazumeva da su kolona i id igraca validni.
 * 
 *  gameBoard* board - pokazivac na tablu
 *  int col          - izabrana kolona, mora biti u opsegu [0,6]
 *  int player       - broj igraca, mora biti 1 ili 2
*/
void makeMove(gameBoard* board, int col, int player) {

    // Zeton igraca player se dodaje u tabelu i azurira se slobodna i-pozicija u toj koloni 
    board->tokens[board->topCol[col]--][col].player = player;
}

/**
 *  Vraca pokazivac na praznu tablu. Svakom zetonu koji ce se eventualno naci u tabli
 *  vec su podesene koordinate.
 *  
 *  float x, y  - koordinate centra prednjeg kruga u ravni za gornji levi zeton
 *  float korak - korak za svaku koordinatu do susednog zetona, pozitivna vrednost
*/
gameBoard gameBoardInit(float x, float y, float slotStep) {
    
    gameBoard board;
    /*
    gameBoard* board = malloc(sizeof(gameBoard*));
    if(board == NULL) {
        fprintf(stderr, "gameBoardInit() malloc fail\n");
        exit(EXIT_FAILURE);
    }*/

    // Alocira se prostor za sve celije table 6x7
    board.tokens = malloc(6*sizeof(token**));
    
    if(board.tokens == NULL) {
        fprintf(stderr, "gameBoardInit() malloc fail\n");
        exit(EXIT_FAILURE);
    }

    int i;
    for(int i=0; i<6; i++) {
        board.tokens[i] = malloc(7*sizeof(token));
        if(board.tokens[i] == NULL) {
            fprintf(stderr, "gameBoardInit() malloc fail\n");
            exit(EXIT_FAILURE);
        }
    }

    // Alocira se niz topCol
    board.topCol = malloc(6*sizeof(int));
    if(board.topCol == NULL) {
        fprintf(stderr, "gameBoardInit() malloc fail\n");
        exit(EXIT_FAILURE);
    }

    // Podesavaju se koordinate za svaki zeton u tabli.
    int j;
    float tokenX, tokenY = y;
    for(i=0; i<6; i++, tokenY -= slotStep) {
        tokenX = x;
        for(j=0; j<7; j++, tokenX += slotStep) {
            board.tokens[i][j].x = tokenX;
            board.tokens[i][j].y = tokenY;

            /* Zeton sa player = 0 se nece crtati jer u trenutnoj igri ne postoji,
               vec samo u memoriji. 
               Na pocetku je cela tabla prazna pa svaki zeton ima vrednost 0 za token.player
            */
            board.tokens[i][j].player = 0;
        }
    }

    // Na pocetku su sve kolone slobodne pa ce zeton padati na dno table, odnosno red sa indeksom 5
    for(j=0; j<7; j++)
        board.topCol[j] = 5;

    return board;
}

/**
 *  Oslobadja memoriju na koju pokazuje pokazivac na tablu 
*/
void freeGameBoard(gameBoard* board) {
    int i;
    for(i=0; i<6; i++)
        free(board->tokens[i]);
    free(board->tokens);
    
    free(board->topCol);
}