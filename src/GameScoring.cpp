#include "GameScoring.h"
#include "Constants.h"

const vector< pair<ScoreState, int> > GameScoring::scoreTable = {
    { ScoreState(1, 0, 0), 100 }, // Single
    { ScoreState(2, 0, 0), 200 }, // Double
    { ScoreState(3, 0, 0), 300 }, // Triple
    { ScoreState(0, 1, 0), 400 }, // T-Spin
    { ScoreState(4, 0, 0), 800 }, // Tetris
    { ScoreState(1, 1, 0), 800 }, // T-Spin Single
    { ScoreState(1, 1, 1), 1200 }, // BtB T-Spin Single
    { ScoreState(4, 0, 1), 1200 }, // BtB Tetris 
    { ScoreState(2, 1, 0), 1200 }, // T-Spin Double
    { ScoreState(3, 1, 0), 1600 }, // T-Spin Triple
    { ScoreState(2, 1, 1), 1800 }, // BtB T-Spin Double
    { ScoreState(3, 1, 1), 2400 } // BtB T-Spin Triple
};

GameScoring::GameScoring() {
    score = 0;
    combo = 0;
    isLastMoveRotate = false;
    isLastLockDifficult = false;
}

GameScoring::~GameScoring() {
}

void GameScoring::handleSoftDropPerCell() {
    score += 1; // 1 point per cell
    debug();
}

void GameScoring::handleHardDropPerCell() {
    score += 2; // 2 points per cell
    debug();
}

void GameScoring::handleScore(int numLineClear, int topLeftHeight, int topLeftWidth, Tile &currentTile, Board &board) {
    // isTSpin and isBtB
    bool isTSpin = checkTSpin(topLeftHeight, topLeftWidth, currentTile, board);
    bool isThisLockDifficult = isLockDifficult(numLineClear, isTSpin);
    bool isBtB = checkBtB(isThisLockDifficult);

    updateCombo(numLineClear);
    updateIsLastLockDifficult(isThisLockDifficult);

    // handle score now!
    score += combo * 50;
    ScoreState currentScoreState = ScoreState(numLineClear, isTSpin, isBtB);
    for (int i = 0; i < (int) scoreTable.size(); ++i) {
        if (currentScoreState == scoreTable[i].first) {
            score += scoreTable[i].second; // add score
            return;
        }
    }
    debug();
}

void GameScoring::updateCombo(int numLineClear) {
    if (numLineClear) combo++;
    else combo = 0;
}

bool GameScoring::isLockDifficult(int numLineClear, bool isTSpin) {
    return numLineClear == 4 || (isTSpin && numLineClear);
}

void GameScoring::updateIsLastLockDifficult(bool isThisLockDifficult) {
    isLastLockDifficult = isThisLockDifficult;
}

bool GameScoring::checkTSpin(int topLeftHeight, int topLeftWidth, Tile &currentTile, Board &board) {
    // 1. Tetromino being locked is T.
    if (currentTile.getType() != T) return false;
    
    // 2. Last successful movement of the tetromino was a rotate, as opposed to sideways movement, downward movement, 
    // or falling due to gravity. The tetromino doesn't even have to end up in a different orientation than it was dropped in.
    if (!isLastMoveRotate) return false;

    // 3. Three of the 4 squares diagonally adjacent to the T's center are occupied. 
    // In our game, the walls and floor surrounding the playfield are considered "occupied".
    int numOccupied = 0;
    int centerHeight = topLeftHeight + 1, centerWidth = topLeftWidth + 1;
    for (int diffHeight = -1; diffHeight <= 1; ++diffHeight) {
        for (int diffWidth = -1; diffWidth <= 1; ++diffWidth) {
            if (diffHeight == 0 || diffWidth == 0) continue; // only -1 and +1
            int height = centerHeight + diffHeight;
            int width = centerWidth + diffWidth;
            numOccupied += (isWallOrFloor(height, width) || board[height][width].getType() != EMPTY);
        }
    }
    if (numOccupied != 3) return false;

    return true; 
}

bool GameScoring::checkBtB(bool isThisLockDifficult) {
    // Back-to-Back (BtB) clears are any combination of two or more 
    // "difficult" line clears without an "easy" line clear between them.
    return (isLastLockDifficult && isThisLockDifficult);
}

bool GameScoring::isWallOrFloor(int height, int width) {
    return height == Constants::BOARD_HEIGHT || height == -1 || width == Constants::BOARD_WIDTH || width == -1;
}

void GameScoring::debug() {
    printf("score=%d combo=%d isLastMoveRotate=%d isLastLockDifficult=%d\n", score, combo, isLastMoveRotate, isLastLockDifficult);
}