#include <stdio.h>  // printf
#include <stdlib.h> // exit
#include <limits.h> // INT_MAX, UINT_MAX, ...
#define MAXIMUM_SITUATIONS 2000
#define ALREADY_COUNTER_MOD 100000
#define MIRRORED_COUNTER_MOD 100000
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

int registeredSituations = 0;
int alreadyCounter = 0;

struct gamesituation {
    char board[BOARD_WIDTH][BOARD_HEIGHT];
    int isEmpty;
    int isFinished;
};

struct gamesituation database[MAXIMUM_SITUATIONS];

void mirrorBoard(char board[BOARD_WIDTH][BOARD_HEIGHT],
                 char newBoard[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int x=0; x <BOARD_WIDTH; x++) {
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
    while (xTemp > 0) {
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
    while (yTemp > 0) {

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
    while (xTemp > 0 && yTemp > 0) {

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
    while (xTemp < BOARD_WIDTH && yTemp > 0) {

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
    while (xTemp > 0 && yTemp < BOARD_HEIGHT) {
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
    unsigned int index;
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

unsigned int hash(unsigned int indexOriginal, unsigned int i) {
    return (indexOriginal + i) % MAXIMUM_SITUATIONS;
}

/** The hashfunction. Tries to find a new slot */
unsigned int getNewIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = getFirstIndex(board);
    unsigned int indexOriginal = index;
    unsigned int i = 1;
    while (!database[index].isEmpty) {
        index = hash(indexOriginal, i);
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
        index = hash(originalIndex, i);
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

void setBoard(unsigned int insertID, char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    database[insertID].isEmpty = FALSE;
    for (int x=0; x<BOARD_WIDTH; x++) {
        for (int y=0; y<BOARD_HEIGHT; y++) {
            database[insertID].board[x][y] = board[x][y];
        }
    }
}

void makeTurns(char board[BOARD_WIDTH][BOARD_HEIGHT], char currentPlayer, unsigned int lastId, int recursion) {
    (void) lastId;
    if (recursion < SHOW_RECURSION_LEVEL) {
        printf("rec-level:%i\n", recursion);
    }
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
            if (alreadyCounter % ALREADY_COUNTER_MOD == 0) {
                printf("did already occur: %i\n", alreadyCounter);
            }
            // I've already got to this situation
            insertID = getMyIndex(board);
        } else {
            char mirrored[BOARD_WIDTH][BOARD_HEIGHT];
            for (int x = 0; x<BOARD_WIDTH; x++) {
                for (int y=0; y<BOARD_HEIGHT; y++) {
                    mirrored[BOARD_WIDTH-x-1][y] = board[x][y];
                }
            }

            if (didBoardAlreadyOccur(mirrored)) {
                // I've already got this situation, but mirrored
                // so take care of symmetry at this point
                insertID = getMyIndex(mirrored);
            } else {
                registeredSituations++;
                if (registeredSituations == MAXIMUM_SITUATIONS) {
                    printf("########################Finish:\n");
                    printf("Maximum of %i reached\n", MAXIMUM_SITUATIONS);
                    printf("alreadyCounter: %i\n", alreadyCounter);
                    exit(10);
                }
                if (registeredSituations % REGISTERED_MOD == 0) {
                    printf("abcdefghijklm");
                }
                outcome = isBoardFinished(board, column, height);
                if (-1 <= outcome && outcome <= 1) { // the game is finished
                    insertID = getNewIndex(board);
                    setBoard(insertID, board);
                } else {
                    // Switch players
                    if (currentPlayer == RED) {
                        currentPlayer = BLACK;
                    } else {
                        currentPlayer = RED;
                    }
                    insertID = getNewIndex(board);
                    setBoard(insertID, board);
                    //savePreviousID(insertID, lastId, column);
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
    for (int i = 0; i < MAXIMUM_SITUATIONS; i++) {
        struct gamesituation x;
        database[i] = x;
        database[i].isEmpty = TRUE;
        database[i].isFinished = FALSE;
        for (int x=0; x < BOARD_WIDTH; x++) {
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

    makeTurns(board, RED, getNewIndex(board), 0);
    return 0;
}
