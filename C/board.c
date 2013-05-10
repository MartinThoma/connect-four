char isSameBoard(char a[BOARD_WIDTH][BOARD_HEIGHT], 
                 char b[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (a[x][y] != b[x][y]) {
                return FALSE;
            }
        }
    }

    return TRUE;
}

void mirrorBoard(char board[BOARD_WIDTH][BOARD_HEIGHT],
                 char newBoard[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            newBoard[BOARD_WIDTH - x - 1][y] = board[x][y];
        }
    }
}

void printBoard(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        printf("# %i ", y + 1);

        for (int x = 0; x < BOARD_WIDTH; x++) {
            printf("%c", board[x][y]);
        }

        printf(" #\n");
    }

    printf("#   1234567 #\n");
    printf("#############\n");
}
