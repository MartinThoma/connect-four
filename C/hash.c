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

    for (unsigned int i = 0; i < n; i++) {
        power *= base;
    }

    return power;
}

unsigned int getFirstIndex(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = 0;

    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            index += charToInt(board[x][y]) *
                     myPow(3, ((x + y * BOARD_WIDTH) % HASH_MODULO));
        }
    }

    index = index % MAXIMUM_SITUATIONS;
    return index;
}

unsigned int secondHashFunction(char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    unsigned int index = 0;

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            index += (2-charToInt(board[x][y])) *
                     myPow(3, ((x + y * BOARD_WIDTH) % HASH_MODULO));
        }
    }

    index = index % MAXIMUM_SITUATIONS;
    return index;
}

unsigned int roundUpSquaredHalf(unsigned int i) {
    return ((float)i / 2.0f + 0.5f);
}

unsigned int linear(unsigned int indexOriginal, unsigned int i, char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    (void) board;
    probingCounter++;
    return (indexOriginal + i) % MAXIMUM_SITUATIONS;
}

unsigned int quadratic(unsigned int indexOriginal, unsigned int i, char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    (void) board;
    probingCounter++;
    return (indexOriginal + ((unsigned int) myPow(-1, i + 1)) * roundUpSquaredHalf(i)*roundUpSquaredHalf(i)) % MAXIMUM_SITUATIONS;
}

unsigned int doubleHashing(unsigned int indexOriginal, unsigned int i, char board[BOARD_WIDTH][BOARD_HEIGHT]) {
    probingCounter++;
    return (indexOriginal + secondHashFunction(board)*i) % MAXIMUM_SITUATIONS;
}
