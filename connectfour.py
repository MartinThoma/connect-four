def didLastPlayerWin(board, lastMove):
    x, y = lastMove
    color = board[x][y]

    # check left-right
    tokensInRow = 1
    xTemp = x - 1
    while xTemp > 0:
        if (board[xTemp][y] != color) {
            break
        } else {
            tokensInRow++;
        }
        xTemp -= 1
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
        winner = lastMove;
        return true;
    }

    # top-down
    tokensInRow = 1;
    yTemp = y - 1;
    while (yTemp > 0) {

        if (board[x][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        yTemp -= 1
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
        winner = lastMove;
        return true;
    }

    # down-left to top-right
    tokensInRow = 0;
    xTemp = x + 1;
    yTemp = y + 1;
    while (xTemp < BOARD_WIDTH and yTemp < BOARD_HEIGHT) {

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
        xTemp -= 1
        yTemp -= 1
    }
    if (tokensInRow >= WINNING_NR) {
        winner = lastMove;
        return true;
    }

    # top-left to down-right
    tokensInRow = 1;
    xTemp = x + 1;
    yTemp = y - 1;
    while (xTemp < BOARD_WIDTH and yTemp > 0) {

        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp++;
        yTemp -= 1
    }
    xTemp = x - 1;
    yTemp = y + 1;
    while (xTemp > 0 and yTemp < BOARD_HEIGHT) {
        if (board[xTemp][yTemp] != color) {
            break;
        } else {
            tokensInRow++;
        }
        xTemp -= 1
        yTemp++;
    }
    if (tokensInRow >= WINNING_NR) {
        winner = lastMove;
        return true;
    }

    return false;
