#define _POSIX_SOURCE // needed for signal handler
#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <limits.h> // INT_MAX, UINT_MAX, ...
#include <signal.h> // for signal handling
#include <time.h> // for clock()
#define MAXIMUM_SITUATIONS 20000000
#define ALREADY_COUNTER_MOD (MAXIMUM_SITUATIONS/20)
#define MIRRORED_COUNTER_MOD 1000
#define REGISTERED_MOD 1
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6
#define SHOW_RECURSION_LEVEL 0
#define WINNING_NR 4
#define TRUE 1
#define FALSE 0
#define RED 'r'
#define BLACK 'b'
#define EMPTY ' '
#define HASH_MODULO 18
#define ABS(a) (a < 0 ? -a : a)
#define PROBING linear //linear
#define GET_STATUS FALSE

unsigned int registeredSituations = 0;
unsigned int alreadyCounter = 0;
unsigned int mirroredCounter = 0;
unsigned long hashMissCounter = 0;
float startTime = -1;

struct gamesituation {
    char board[BOARD_WIDTH][BOARD_HEIGHT];
    int next[7];
    int isEmpty;
    int winRed;
    int winBlack;
    int stalemate;
    int isFinished;
};

struct gamesituation database[MAXIMUM_SITUATIONS];

void giveCurrentInformation() {
    printf("  alreadyCounter: %i\n", alreadyCounter);
    printf("  mirroredCounter: %i\n", mirroredCounter);
    printf("  hashMiss: %lu\n", hashMissCounter);
}

void sigint_handler(int sig) {
    float endTime = (float)clock()/CLOCKS_PER_SEC;
    if ((endTime - startTime) <= 2) {
        printf("Shutting down\n");
        exit(1);
    }
    startTime = (float)clock()/CLOCKS_PER_SEC;
    printf("Received Signal\n");
    giveCurrentInformation();
    printf("Status: %i of %i\n\n", registeredSituations, MAXIMUM_SITUATIONS);
    (void) sig;
}

void mirrorBoard(char board[BOARD_WIDTH][BOARD_HEIGHT],
                 char newBoard[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int x=0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            newBoard[BOARD_WIDTH - x - 1][y] = board[x][y];
        }
    }
}

int isBoardFinished(char board[BOARD_WIDTH][BOARD_HEIGHT], int x, int y) {
    char color = board[x][y];

    // check left-right
    int tokensInRow = 1;
    int xTemp = x - 1;
    while (xTemp >= 0) {
        if (board[xTemp][y] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp--;
    }
    xTemp = x + 1;
    while (xTemp < BOARD_WIDTH) {
        if (board[xTemp][y] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp++;
    }

    if (tokensInRow >= WINNING_NR) {
        if (color == RED) {
            return 1;
        } else if (color == BLACK) {
            return -1;
        } else {
            printf("abnormal exit\n");
            exit(1);
        }
    }

    // top-down
    tokensInRow = 1;
    int yTemp = y - 1;
    while (yTemp >= 0) {

        if (board[x][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        yTemp--;
    }
    yTemp = y + 1;
    while (yTemp < BOARD_HEIGHT) {

        if (board[x][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        yTemp++;
    }

    if (tokensInRow >= WINNING_NR) {
        if (color == RED) {
            return 1;
        } else if (color == BLACK) {
            return -1;
        } else {
            printf("abnormal exit2\n");
            exit(2);
        }
    }

    // down-left to top-right
    tokensInRow = 0;
    xTemp = x + 1;
    yTemp = y + 1;
    while (xTemp < BOARD_WIDTH && yTemp < BOARD_HEIGHT) {

        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp++;
        yTemp++;
    }
    xTemp = x - 1;
    yTemp = y - 1;
    while (xTemp >= 0 && yTemp >= 0) {

        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp--;
        yTemp--;
    }
    if (tokensInRow >= WINNING_NR) {
        if (color == RED) {
            return 1;
        } else if (color == BLACK) {
            return -1;
        } else {
            printf("abnormal exit3\n");
            exit(3);
        }
    }

    // top-left to down-right
    tokensInRow = 1;
    xTemp = x + 1;
    yTemp = y - 1;
    while (xTemp < BOARD_WIDTH && yTemp >= 0) {

        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp++;
        yTemp--;
    }
    xTemp = x - 1;
    yTemp = y + 1;
    while (xTemp >= 0 && yTemp < BOARD_HEIGHT) {
        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp--;
        yTemp++;
    }
    if (tokensInRow >= WINNING_NR) {
        if (color == RED) {
            return 1;
        } else if (color == BLACK) {
            return -1;
        } else {
            printf("abnormal exit4\n");
            exit(4);
        }
    }

    if (board[0][5] != EMPTY && board[1][5] != EMPTY && board[2][5] != EMPTY &&
            board[3][5] != EMPTY && board[4][5] != EMPTY && board[5][5] != EMPTY &&
            board[6][5] != EMPTY) {
        return 0;
    }

    return 100;
}

unsigned int charToInt(char x) {
    if (x == RED) {
        return 1;
    } else if (x == BLACK) {
        return 2;
    } else {
        return 0;
    }
}

unsigned int myPow(int base, unsigned int n) {
    unsigned int power = 1;
    for (unsigned int i=0; i<n; i++) {
        power *= base;
    }
    return power;
}

unsigned int getFirstIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = 0;
    for (int x=0; x<BOARD_WIDTH; x++) {
        for (int y=0; y<BOARD_HEIGHT; y++) {
            index += charToInt(board[x][y])*myPow(3, ((x+y*BOARD_WIDTH)%HASH_MODULO));
        }
    }
    index = index % MAXIMUM_SITUATIONS;
    return index;
}

unsigned int roundUpSquaredHalf(unsigned int i) {
    return ((float)i / 2.0f + 0.5f);
}

unsigned int quadratic(unsigned int indexOriginal, unsigned int i) {
    hashMissCounter++;
    return (indexOriginal + ((unsigned int) myPow(-1, i+1)) * roundUpSquaredHalf(i)) % MAXIMUM_SITUATIONS;
}

unsigned int linear(unsigned int indexOriginal, unsigned int i) {
    hashMissCounter++;
    return (indexOriginal + i) % MAXIMUM_SITUATIONS;
}

/** The hashfunction. Tries to find a new slot */
unsigned int getNewIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = getFirstIndex(board);
    unsigned int indexOriginal = index;
    unsigned int i = 1;
    while (!database[index].isEmpty) {
        index = PROBING(indexOriginal, i);
        i++;
    }

    return index;
}

int isSameBoard(char a[BOARD_WIDTH][BOARD_HEIGHT], char b[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int x=0; x<BOARD_WIDTH; x++) {
        for (int y=0; y<BOARD_HEIGHT; y++) {
            if (a[x][y] != b[x][y]) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/** This function returns the index of a board that is already in
 *  the situation array. If it is not there, it returns 0.
 */
unsigned int getMyIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int originalIndex = getFirstIndex(board);
    unsigned int index = originalIndex;
    unsigned int i = 1;
    while (!database[index].isEmpty && !isSameBoard(board, database[index].board)) {
        index = PROBING(originalIndex, i);
        i++;
        if (isSameBoard(board, database[index].board)) {
            return index;
        }
    }
    return isSameBoard(board, database[index].board); // can be both: its not there and its at 0!
}

int didBoardAlreadyOccur(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = getMyIndex(board);
    if (index == 0) {
        return isSameBoard(board, database[index].board);
    } else {
        return TRUE;
    }
}

void savePreviousID(unsigned int insertID, unsigned int lastId, unsigned int column) {
    // speichere nun die ID in der vorgänger-ID
    database[lastId].next[column] = insertID;
}

void setBoard(unsigned int insertID, char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    database[insertID].isEmpty = FALSE;
    for (int x=0; x<BOARD_WIDTH; x++) {
        for (int y=0; y<BOARD_HEIGHT; y++) {
            database[insertID].board[x][y] = board[x][y];
        }
    }
}

void printBoard(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int y=BOARD_HEIGHT-1; y>=0; y--) {
        printf("# %i ", y+1);
        for (int x=0; x<BOARD_WIDTH; x++) {
            printf("%c", board[x][y]);
        }
        printf(" #\n");
    }
    printf("#   1234567 #\n");
    printf("#############\n");
}

void storeToDatabase(int insertID, char board[BOARD_WIDTH][BOARD_HEIGHT], int isFinished, int outcome) {
    setBoard(insertID, board);
    if (isFinished) {
        database[insertID].isFinished = isFinished;
        if (outcome == 0) {
            database[insertID].stalemate = TRUE;
        } else if (outcome == 1) {
            database[insertID].winRed = TRUE;
        } else {
            database[insertID].winBlack = TRUE;
        }
    }
}

void makeTurns(char board[BOARD_WIDTH][BOARD_HEIGHT], char currentPlayer, unsigned int lastId, int recursion) {
    #if GET_STATUS
    if (recursion < SHOW_RECURSION_LEVEL) {
        printf("rec-level:%i\n", recursion);
    }
    #endif

    unsigned int insertID;
    int outcome;

    for (int column=0; column<BOARD_WIDTH; column++) {
        // add to column
        int height = BOARD_HEIGHT - 1;
        while (height >= 0 && board[column][height] == EMPTY) {
            height--;
        }
        height++;
        if (height == 6) {
            continue;
        }

        // place tile
        board[column][height] = currentPlayer;

        if (didBoardAlreadyOccur(board)) {
            alreadyCounter++;
            #if GET_STATUS
                if (alreadyCounter % ALREADY_COUNTER_MOD == 0) {
                    printf("did already occur: %i\n", alreadyCounter);
                }
            #endif

            // I've already got to this situation
            insertID = getMyIndex(board);
            savePreviousID(insertID, lastId, column);
        } else {
            char mirrored[BOARD_WIDTH][BOARD_HEIGHT];
            mirrorBoard(board, mirrored);

            if (didBoardAlreadyOccur(mirrored)) {
                // I've already got this situation, but mirrored
                // so take care of symmetry at this point
                mirroredCounter++;
                #if GET_STATUS
                    if (mirroredCounter % MIRRORED_COUNTER_MOD == 0) {
                        printf("mirrored: %i\n", mirroredCounter);
                    }
                #endif
                insertID = getMyIndex(mirrored);
                savePreviousID(insertID, lastId, column);
            } else {
                registeredSituations++;
                if (registeredSituations == MAXIMUM_SITUATIONS) {
                    giveCurrentInformation();
                    exit(10);
                }
                if (registeredSituations % REGISTERED_MOD == 0) {
                    //printf("registeredSituations:\t%i\n", registeredSituations);
                    //printf("abcdefghijklm");
                }
                outcome = isBoardFinished(board, column, height);
                if (ABS(outcome) <= 1) { // the game is finished
                    insertID = getNewIndex(board);
                    storeToDatabase(insertID, board, TRUE, outcome);
                    savePreviousID(insertID, lastId, column);
                } else {
                    // Switch players
                    if (currentPlayer == RED) {
                        currentPlayer = BLACK;
                    } else {
                        currentPlayer = RED;
                    }
                    insertID = getNewIndex(board);
                    setBoard(insertID, board);
                    savePreviousID(insertID, lastId, column);
                    char copy[BOARD_WIDTH][BOARD_HEIGHT];
                    for (int x =0; x<BOARD_WIDTH; x++) {
                        for (int y=0; y<BOARD_HEIGHT; y++) {
                            copy[x][y] = board[x][y];
                        }
                    }
                    makeTurns(copy, currentPlayer, insertID, recursion+1);
                }
            }
        }
    }
}

int main() {
    /* If the user wants to quit the calculation early, give
       the results by now */
    void sigint_handler(int sig);
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    /* Output characteristics of this run */
    printf("Opitimization: O3\n");
    printf("Probing: linear\n");
    printf("Maximum: %i\n", MAXIMUM_SITUATIONS);

    #if GET_STATUS
        printf("Started initialisation\n");
    #endif
    for (int i = 0; i < MAXIMUM_SITUATIONS; i++) {
        database[i].isEmpty = TRUE;
        database[i].isFinished = FALSE;
        database[i].stalemate = FALSE;
        database[i].winRed = FALSE;
        database[i].winBlack = FALSE;
        for (int x=0; x < BOARD_WIDTH; x++) {
            database[i].next[x] = 0;
            for (int y=0; y<BOARD_HEIGHT; y++) {
                database[i].board[x][y] = EMPTY;
            }
        }
    }

    char board[BOARD_WIDTH][BOARD_HEIGHT];
    for (int x=0; x < BOARD_WIDTH; x++) {
        for (int y=0; y<BOARD_HEIGHT; y++) {
            board[x][y] = EMPTY;
        }
    }

    #if GET_STATUS
        printf("Started makeTurns\n");
    #endif

    makeTurns(board, RED, getNewIndex(board), 0);
    printf("Found %i Situations\n", registeredSituations);
    printf("END\n");
    return 0;
}
