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
 *  char player       - broj igraca, mora biti '1' ili '2'
*/
void makeMove(gameBoard* board, int col, char player) {

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

    // Alocira se prostor za niz topCol
    board.topCol = malloc(7*sizeof(short));
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

            /* Zeton sa player = '0' se nece crtati jer u trenutnoj igri ne postoji,
               vec samo u memoriji. 
               Na pocetku je cela tabla prazna pa svaki zeton ima vrednost '0' za token.player
            */
            board.tokens[i][j].player = '0';
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

/**
 *  Mapira oznake igraca za svaki zeton u matricu karaktera
*/
state* boardToState(gameBoard* board) {
    state* state = malloc(sizeof(state));
    if(state == NULL) {
        fprintf(stderr, "boardToState() malloc fail\n");
        exit(EXIT_FAILURE);
    }

    state->st = malloc(6 * sizeof(char*));
    if(state->st == NULL) {
        fprintf(stderr, "boardToState() malloc fail\n");
        exit(EXIT_FAILURE);
    }
    int i, j;
    for(i=0; i<6; i++) {
        state->st[i] = malloc(7);
        if(state->st[i] == NULL) {
            fprintf(stderr, "boardToState() malloc fail\n");
            exit(EXIT_FAILURE);
        }

        for(j=0; j<7; j++)
            state->st[i][j] = board->tokens[i][j].player;
    }
    
    state->top = malloc(7*sizeof(short));
    if(state->top == NULL) {
        fprintf(stderr, "boardToState() malloc fail\n");
        exit(EXIT_FAILURE);
    }

    for(j=0; j<7; j++)
        state->top[j] = board->topCol[j];

    return state;
}

/**
 *  Oslobadja memoriju koju zauzima state* state
*/
void freeState(state* state){
    int i;
    for(i=0; i<6; i++)
        free(state->st[i]);
    free(state->st);
    
    free(state->top);

    free(state);
}

/**
 *  Funkcija evaluacije stanja igre.
 * 
 *  Vraca 1000 ili -1000 ako je pobednik 1. igrac ili 2.igrac/racunar,
 *  ili po dodatnim kriterijumima racuna poene.
*/
int evaluate(state* state) {
    /*
        Proverava se da li ima 4 u nizu po redovima.
        Ako u i-tom redu u koloni j=3 nema zetona, nema sigurno pobednika u tom redu,
        pa ni u gornjim redovima.
    */
    int i,j;
    for(i=5; i>=0 && state->st[i][3] != '0'; i--)
        for(j=3; j<7; j++)
            if(state->st[i][j] == state->st[i][j-1] && state->st[i][j-1] == state->st[i][j-2] &&
                state->st[i][j-2] == state->st[i][j-3]) {
                
                return state->st[i][j] == '1' ? 1000 : -1000;
            }
    /*
        Proverava se po kolonama.
        Ako na polju (i,j) nema zetona, tu nema 4 u nizu, sece se i pretraga za vise redove
    */
    for(j=0; j<7; j++)
        for(i=2; i>=0 && state->st[i][j] != '0'; i--)
            if(state->st[i][j] == state->st[i+1][j] && state->st[i+1][j] == state->st[i+2][j] &&
                state->st[i+2][j] == state->st[i+3][j]) {
                
                return state->st[i][j] == '1' ? 1000 : -1000;
            }

    // Slicno za dijagonale oblika '/'
    for(j=3; j<7; j++)
        for(i=2; i>=0 && state->st[i][j] != '0'; i--)
            if(state->st[i][j] == state->st[i+1][j-1] && state->st[i+1][j-1] == state->st[i+2][j-2] &&
                state->st[i+2][j-2] == state->st[i+3][j-3]) {
                
                return state->st[i][j] == '1' ? 1000 : -1000;
            }

    // Slicno za dijagonale oblika '\'
    for(j=3; j>=0; j--)
        for(i=2; i>=0 && state->st[i][j] != '0'; i--)
            if(state->st[i][j] == state->st[i+1][j+1] && state->st[i+1][j+1] == state->st[i+2][j+2] &&
                state->st[i+2][j+2] == state->st[i+3][j+3]) {
                
                return state->st[i][j] == '1' ? 1000 : -1000;
            }

    /*
        Proverava se da li postoje 3 u nizu i bar jedna pozicija sa neke strane gde se moze
        odigrati za pobedu. 
        
        Npr za horizontalu, 3 u nizu za j=[1,2,3] u redu i:
        Ako je samo jedan od top[0] i top[4] jednak 0, to nosi 5 odnosno -5 poena.
        Ako dodatno vazi top[0]=top[4]=i, to je "mat" i nosi 50/-50 poena.

        Medjutim potrebno je prvo proveriti sve horizontale, i dijagonale za slucajeve 8/-8
        pa tek onda za slucajeve 5/-5. Dodatno postoje posebni slucajevi na dijagonalama i
        svim kolonama gde nije moguce osvojiti 50/-50.

        Posto se ovakvi slucajevi mogu ponavljati za oba igraca u jednom istom stanju,
        poeni se sabiraju.
    */

    int score = 0;

    // Prvo se gleda horizontalno za 50/-50
    for(i=5; i>=0 && state->st[i][3] != '0'; i--)
        for(j=3; j<6; j++)
            if(state->st[i][j] == state->st[i][j-1] && state->st[i][j-1] == state->st[i][j-2] &&
                state->top[j-3]==i && state->top[j+1]==i) {

                score += state->st[i][j] == '1' ? 50 : -50;
            }

    // Slicno za dijagonale oblika '/'
    for(j=3; j<6; j++)
        for(i=2; i>0; i--)
            if(state->st[i][j]!='0' && state->st[i][j] == state->st[i+1][j-1] && 
                state->st[i+1][j-1] == state->st[i+2][j-2] && state->top[j-3]==i+3 && 
                    state->top[j+1]==i-1) {
                
                score += state->st[i][j] == '1' ? 50 : -50;
            }

    // Slicno za dijagonale oblika '\'
    for(j=3; j<6; j++)
        for(i=4; i>2; i--)
            if(state->st[i][j]!='0' && state->st[i][j] == state->st[i-1][j-1] && 
                state->st[i-1][j-1] == state->st[i-2][j-2] && state->top[j+1]==i+1 && 
                    state->top[j-3]==i-3) {
                
                score += state->st[i][j] == '1' ? 50 : -50;
            }

    // Traze se slucajevi za 5/-5 poena.
    
    // Opsti slucajevi za horizontale
    for(i=5; i>=0 && state->st[i][3] != '0'; i--)
        for(j=3; j<6; j++)
            if(state->st[i][j] == state->st[i][j-1] && state->st[i][j-1] == state->st[i][j-2] &&
                (state->top[j-3]==i || state->top[j+1]==i)) {

                score += state->st[i][j] == '1' ? 5 : -5;
            }
    //  Specijalni slucajevi za horizontale
    for(i=5; i>=0; i--) {
        if(state->st[i][0]!='0' && state->st[i][0] == state->st[i][1] && 
            state->st[i][1] == state->st[i][2] && state->top[3]==i) {

            score += state->st[i][0] == '1' ? 5 : -5;
        }

        if(state->st[i][6]!='0' && state->st[i][6] == state->st[i][5] && 
            state->st[i][5] == state->st[i][4] && state->top[3]==i) {

            score += state->st[i][6] == '1' ? 5 : -5;
        }
    }

    // Kolone ne mogu dati 50/-50 ni u kom slucaju
    for(j=0; j<7; j++)
        for(i=3; i>0 && state->st[i][j] != '0'; i--)
            if(state->st[i][j] == state->st[i+1][j] && state->st[i+1][j] == state->st[i+2][j] &&
                state->top[j]==i-1) {
                
                score += state->st[i][j] == '1' ? 5 : -5;
            } 
    
    // Dijagonale oblika '/'
    for(j=3; j<6; j++)
        for(i=2; i>0; i--)
            if(state->st[i][j]!='0' && state->st[i][j] == state->st[i+1][j-1] && 
                state->st[i+1][j-1] == state->st[i+2][j-2] && (state->top[j-3]==i+3 || 
                    state->top[j+1]==i-1)) {
                
                score += state->st[i][j] == '1' ? 5 : -5;
            }
    // Dijagonale oblika '\'
    for(j=3; j<6; j++)
        for(i=4; i>2; i--)
            if(state->st[i][j]!='0' && state->st[i][j] == state->st[i-1][j-1] && 
                state->st[i-1][j-1] == state->st[i-2][j-2] && (state->top[j+1]==i+1 || 
                    state->top[j-3]==i-3)) {
                
                score += state->st[i][j] == '1' ? 5 : -5;
            }

    // Specijalni slucajevi za sve dijagonale uz levi i desni rub
    for(i=3; i>0; i--) {
        // Oblik '/' levi rub
        if(state->st[i][2]!='0' && state->st[i][2] == state->st[i+1][1] && 
            state->st[i+1][1] == state->st[i+2][0] && state->top[3]==i-1) {

            score += state->st[i][2] == '1' ? 5 : -5;
        }

        // Oblik '\' levi rub
        if(state->st[i+1][2]!='0' && state->st[i+1][2] == state->st[i][1] && 
            state->st[i][1] == state->st[i-1][0] && state->top[3]==i+2) {

            score += state->st[i+1][2] == '1' ? 5 : -5;
        }

        // Oblik '/' desni rub
        if(state->st[i+1][4]!='0' && state->st[i+1][4] == state->st[i][5] && 
            state->st[i][5] == state->st[i-1][6] && state->top[3]==i+2) {

            score += state->st[i+1][4] == '1' ? 5 : -5;
        }

        // Oblik '\' desni rub
        if(state->st[i][4]!='0' && state->st[i][4] == state->st[i+1][5] && 
            state->st[i+1][5] == state->st[i+2][6] && state->top[3]==i-1) {

            score += state->st[i][4] == '1' ? 5 : -5;
        }
    }

    // Specijalni slucajevi za sve dijagonale uz gornji i donji rub
    for(j=2; j<6; j++) {
        // Oblik '/' donji rub
        if(state->st[3][j]!='0' && state->st[3][j] == state->st[4][j-1] && 
            state->st[4][j-1] == state->st[5][j-2] && state->top[j+1]==2) {

            score += state->st[3][j] == '1' ? 5 : -5;
        }

        // Oblik '\' donji rub
        if(state->st[5][j+1]!='0' && state->st[5][j+1] == state->st[4][j] && 
            state->st[4][j] == state->st[3][j-1] && state->top[j-2]==2) {

            score += state->st[5][j+1] == '1' ? 5 : -5;
        }

        // Oblik '/' gornji rub
        if(state->st[0][j+1]!='0' && state->st[0][j+1] == state->st[1][j] && 
            state->st[1][j] == state->st[2][j-1] && state->top[j-2]==3) {

            score += state->st[0][j+1] == '1' ? 5 : -5;
        }

        // Oblik '\' gornji rub
        if(state->st[2][j]!='0' && state->st[2][j] == state->st[1][j-1] && 
            state->st[1][j-1] == state->st[0][j-2] && state->top[j+1]==3) {

            score += state->st[2][j] == '1' ? 5 : -5;
        }
    }
    
    return score;
}

/** TOFIX: U nekim slucajevima ne primecuje da protivnik u sledecem potezu pobedjuje
 *  Implementira minimax algoritam sa alfa-beta odsecanjem.
 * 
 *  Vraca strukturu minMax koja sadrzi ocenu stanja i potez koji se bira.
*/
minMax minimax(state* startState, int depth, char player, int alpha, int beta) {
    minMax node;
    node.value = evaluate(startState);
    node.col = startState->lastMove;
    
    // Proverava se da li je puna tabla tj da li su svi top[j]=-1
    int unfinished = 7, j;
    for(j=0; j<7; j++)
        unfinished += startState->top[j];

    // Izlaz iz rekurzije za 0-tu dubinu, ako postoji pobednik ili ako je puna tabla
    if(!depth || node.value || !unfinished)
        return node;
    
    // Generisu se sledeca stanja sa svim mogucim potezima igraca player
    stateArr stArr = getNextStates(startState, player);

    // 1. igrac maksimizira, racunar minimizira
    if(player == '1') {
        node.value = INT_MIN;
        int i;
        for(i=0; i<stArr.size; i++) {
            minMax minVal = minimax(&stArr.a[i], depth-1, '2', alpha, beta);
            
            if(minVal.value > node.value) {
                node.value = minVal.value;
                node.col = stArr.a[i].lastMove;
                printf("1. %d %d\n", node.value, node.col);
            }

            alpha = node.value > alpha? node.value : alpha;
            
            if(beta <= alpha)
                break;
        }
    } else {
        node.value = INT_MAX;
        int i;
        for(i=0; i<stArr.size; i++) {
            minMax maxVal = minimax(&stArr.a[i], depth-1, '1', alpha, beta);
            
            if(maxVal.value < node.value) {
                node.value = maxVal.value;
                node.col = stArr.a[i].lastMove;
                printf("2. %d %d\n", node.value, node.col);
            }

            beta = node.value < beta ? node.value : beta;
            
            if(beta <= alpha)
                break;
        }
    }   
    
    freeStateArr(&stArr);

    return node;
}

/**
 *  Generise nova stanja za f-ju minimax
 * 
 *  Vraca strukturu stateArr koja sadrzi niz stanja i velicinu tog niza.
 *  Matrice karaktera unutar svakog stanja (unutar strukture state) se moraju
 *  naknadno dealocirati, kao i nizovi top, npr. f-jom freeStateArr.
*/
stateArr getNextStates(state* startState, char player) {
    stateArr stArr;

    // Nalazi se broj mogucih novih stanja i alocira se prostor za niz.
    stArr.size = 0;
    int i;
    for(i=0; i<7; i++)
        if(startState->top[i] != -1)
            stArr.size++;
    
    stArr.a = malloc(stArr.size * sizeof(state));
    if(stArr.a == NULL) {
        fprintf(stderr, "getNextStates() malloc fail\n");
        exit(EXIT_FAILURE);
    }
    
    // Pravi se stArr.size duplikata od startState
    for(i=0; i<stArr.size; i++) {
        state next;
        // Kopira se matrica karaktera
        next.st = malloc(6*sizeof(char*));
        if(next.st == NULL) {
            fprintf(stderr, "getNextStates() malloc fail\n");
            exit(EXIT_FAILURE);
        }
    
        int j;
        for(j=0; j<6; j++) {
            next.st[j] = malloc(7);
            if(next.st[j] == NULL) {
                fprintf(stderr, "getNextStates() malloc fail\n");
                exit(EXIT_FAILURE);
            }

            memcpy(next.st[j], startState->st[j], 7);
        }

        // Kopira se niz top
        next.top = malloc(7*sizeof(short));
        if(next.top == NULL) {
            fprintf(stderr, "getNextStates() malloc fail\n");
            exit(EXIT_FAILURE);
        }
        for(j=0; j<7; j++)
            next.top[j] = startState->top[j];

        stArr.a[i] = next;
    }

    // Dodaju se novi odigrani potezi igraca player i azurira niz top    
    int j;
    for(i=0, j=0; j<7; j++) {
        int k = startState->top[j];
        if (k != -1) {
            stArr.a[i].st[k][j] = player;
            stArr.a[i].top[j]--;
            stArr.a[i].lastMove = j;
            i++;
        }
    }

    return stArr;
}

/**
 *  Dealocira sve matrica karaktera stanja na ciji niz pokazuje stArr.
*/
void freeStateArr(stateArr* stArr) {
    int i,j;
    for(i=0; i<stArr->size; i++) {
        for(j=0; j<6; j++)
            free(stArr->a[i].st[j]);
        free(stArr->a[i].st);
        free(stArr->a[i].top);
    }
}