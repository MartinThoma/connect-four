#define _POSIX_SOURCE // needed for signal handler
#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <limits.h> // INT_MAX, UINT_MAX, ...
#include <signal.h> // for signal handling
#include <time.h> // for clock()
#define MAXIMUM_SITUATIONS 20000000
#define ALREADY_COUNTER_MOD (MAXIMUM_SITUATIONS/20)
#define MIRRORED_COUNTER_MOD -1
#define REGISTERED_MOD 10000
#define SHORT_OUTPUT 1
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
#define PROBING doubleHashing // linear, quadratic, doubleHashing
#define GET_STATUS TRUE
#define NOT_FINISHED 100
#define MAXIMUM_SITUATIONS_REACHED_EXIT_STATUS 2
#include "board.c" // board operations

unsigned int registeredSituations = 0;
unsigned int mirroredCounter = 0;
unsigned long probingCounter = 0;
float programStartTime = -1;
float startTime = -1;

struct gamesituation {
    /** How does the board currently look like? */
    char board[BOARD_WIDTH][BOARD_HEIGHT];

    /**
     * What are the next game situations that I can reach from this
     * board?
     * The next[i] means that the player dropped the disc at column i
     */
    int next[7];

    /* I could use a bitfield for this ... but it would make access
     * much more inconvenient.
     */
    unsigned char isEmpty;  // Is this gamesitatution already filled?
    unsigned char isFinished; // Is this game finished?
    unsigned char stalemate; // Was this game a stalemate?
    unsigned char winRed;   // Did red win?
    unsigned char winBlack; // Did black win?
};

struct gamesituation database[MAXIMUM_SITUATIONS];

void giveCurrentInformation() {
    if(SHORT_OUTPUT) {
        printf("%0.2f,", (float)clock() / CLOCKS_PER_SEC - programStartTime);
        printf("%i,", registeredSituations);
        printf("%i,", mirroredCounter);
        printf("%lu\n", probingCounter);
    } else {
        printf("%0.2fs\n", (float)clock() / CLOCKS_PER_SEC - programStartTime);
        printf("\tregisteredSituations:\t%i\n", registeredSituations);
        printf("\tmirroredCounter: %i\n", mirroredCounter);
        printf("\tprobingCounter: %lu\n", probingCounter);
    }
}

void sigint_handler(int sig) {
    float endTime = (float)clock() / CLOCKS_PER_SEC;

    if ((endTime - startTime) <= 2) {
        printf("Shutting down\n");
        exit(1);
    }

    startTime = (float)clock() / CLOCKS_PER_SEC;
    printf("Received Signal\n");
    giveCurrentInformation();
    printf("Status: %i of %i\n\n", registeredSituations, MAXIMUM_SITUATIONS);
    (void) sig;
}

#include "hash.c"

/** The hashfunction. Tries to find a new slot */
unsigned int getNewIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = getFirstIndex(board);
    unsigned int indexOriginal = index;
    unsigned int i = 1;

    while (!database[index].isEmpty) {
        index = PROBING(indexOriginal, i, board);
        i++;
    }

    return index;
}

/* 
 * This function returns the index of a board that is already in
 * the situation array. If it is not there, it returns 0.
 */
unsigned int getBoardIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int originalIndex = getFirstIndex(board);
    unsigned int index = originalIndex;
    unsigned int i = 1;

    while (!database[index].isEmpty && 
           !isSameBoard(board, database[index].board)) {
        index = PROBING(originalIndex, i, board);
        i++;

        if (isSameBoard(board, database[index].board)) {
            return index;
        }
    }

    // can be both: its not there and its at 0!
    return isSameBoard(board, database[index].board); 
}

/********************************************************************
 * Misc                                                             *
 *******************************************************************/
/*
 * Checks if a given board is stored in database.
 * @param board
 * @return TRUE iff board is stored in database, otherwise FALSE
 */
int didBoardAlreadyOccur(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = getBoardIndex(board);

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

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            database[insertID].board[x][y] = board[x][y];
        }
    }
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

int getTokensInRow(char board[BOARD_WIDTH][BOARD_HEIGHT], char color,
                   int x, int y, char xDir, char yDir) {
    int tokensInRow = 1;
    int xTemp = x + xDir;
    int yTemp = y + yDir;

    while (0 <= xTemp && xTemp < BOARD_WIDTH
        && 0 <= yTemp && yTemp < BOARD_HEIGHT) {
        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }

        xTemp += xDir;
        yTemp += yDir;
    }

    return tokensInRow;
}

/*
 * Check if player has won by placing a disc on (x,y). 
 * with direction (xDir, yDir)
 * @return 1 iff RED won, -1 iff BLACK won and 0 if nobody won
 */
signed char hasPlayerWon(char board[BOARD_WIDTH][BOARD_HEIGHT], 
                  int x, int y, char xDir, char yDir) {
    char color = board[x][y];

    int tokensInRow = getTokensInRow(board, color, x, y, xDir, yDir)
              + getTokensInRow(board, color, x, y, -xDir, -yDir) - 1;

    if (tokensInRow >= WINNING_NR) {
        if (color == RED) {
            return 1;
        } else if (color == BLACK) {
            return -1;
        } else {
            perror("this color doesn't / shouldn't exist\n");
            exit(1);
        }
    }

    return 0;
}

/* 
 * A new disc has been dropped. Check if this disc means that 
 * somebody won.
 * @return 1 iff RED won, -1 iff BLACK won, otherwise NOT_FINISHED
 */
int isBoardFinished(char board[BOARD_WIDTH][BOARD_HEIGHT], 
                    int x, int y) {
    signed char status;

    // check left-right
    status = hasPlayerWon(board, x, y, 1, 0);

    if (status != 0) {
        return status;
    }

    // top-down
    status = hasPlayerWon(board, x, y, 0, 1);

    if (status != 0) {
        return status;
    }

    // down-left to top-right
    status = hasPlayerWon(board, x, y, 1, 1);

    if (status != 0) {
        return status;
    }

    // top-left to down-right
    status = hasPlayerWon(board, x, y, -1, 1);

    if (status != 0) {
        return status;
    }

    return NOT_FINISHED;
}


/*
 * Make all possible turns that the player can make in this
 * game situation.
 */
void makeTurns(char board[BOARD_WIDTH][BOARD_HEIGHT], 
    char currentPlayer, unsigned int lastId, int recursion) {
    #if GET_STATUS
        if (recursion < SHOW_RECURSION_LEVEL) {
            printf("rec-level:%i\n", recursion);
        }
    #endif

    unsigned int insertID;
    int outcome;

    for (int column = 0; column < BOARD_WIDTH; column++) {
        // add to column
        int height = BOARD_HEIGHT - 1;

        // the disc falls down
        while (height >= 0 && board[column][height] == EMPTY) {
            height--;
        }
        height++;

        // this colum is full
        if (height == 6) {
            continue;
        }

        // place disc
        board[column][height] = currentPlayer;

        if (didBoardAlreadyOccur(board)) {
            // I've already got to this situation
            insertID = getBoardIndex(board);
            savePreviousID(insertID, lastId, column);
        } else {
            char mirrored[BOARD_WIDTH][BOARD_HEIGHT];
            mirrorBoard(board, mirrored);

            if (didBoardAlreadyOccur(mirrored)) {
                // I've already got this situation, but mirrored
                // so take care of symmetry at this point
                mirroredCounter++;
                #if GET_STATUS
                    if (MIRRORED_COUNTER_MOD > 0 && mirroredCounter % MIRRORED_COUNTER_MOD == 0) {
                        printf("mirrored: %i\n", mirroredCounter);
                    }
                #endif
                insertID = getBoardIndex(mirrored);
                savePreviousID(insertID, lastId, column);
            } else {
                registeredSituations++;

                if (registeredSituations == MAXIMUM_SITUATIONS) {
                    giveCurrentInformation();
                    exit(MAXIMUM_SITUATIONS_REACHED_EXIT_STATUS);
                }

                if (REGISTERED_MOD > 0 &&
                    registeredSituations % REGISTERED_MOD == 0) {
                    giveCurrentInformation();
                }

                outcome = isBoardFinished(board, column, height);

                if (ABS(outcome) <= 1) {
                    // the game is finished
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

                    for (int x = 0; x < BOARD_WIDTH; x++) {
                        for (int y = 0; y < BOARD_HEIGHT; y++) {
                            copy[x][y] = board[x][y];
                        }
                    }

                    makeTurns(copy, currentPlayer, insertID, 
                              recursion + 1);
                }
            }
        }
    }
}

int main() {
    programStartTime = (float)clock() / CLOCKS_PER_SEC;

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

        for (int x = 0; x < BOARD_WIDTH; x++) {
            database[i].next[x] = 0;

            for (int y = 0; y < BOARD_HEIGHT; y++) {
                database[i].board[x][y] = EMPTY;
            }
        }
    }

    char board[BOARD_WIDTH][BOARD_HEIGHT];

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
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
