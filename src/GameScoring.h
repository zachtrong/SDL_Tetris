#pragma once
#include <iostream>
#include <vector>
#include "Board.h"
#include "Tile.h"
using namespace std;

struct ScoreState {
    int numLineClear;
    bool isTSpin;
    bool isBtB;
    ScoreState(int numLineClear, bool isTSpin, bool isBtB):
        numLineClear(numLineClear), isTSpin(isTSpin), isBtB(isBtB) {}
    bool operator == (const ScoreState &other) const {
        return numLineClear == other.numLineClear && isTSpin == other.isTSpin && isBtB == other.isBtB;
    }
};

class GameScoring {
private:
    static const vector < pair<ScoreState, int> > scoreTable;
public:
    int score;
    int combo;
    bool isLastMoveRotate;
    bool isLastLockDifficult;

    GameScoring();
    virtual ~GameScoring();

    void debug();
    void handleScore(int numLineClear, int topLeftHeight, int topLeftWidth, Tile &currentTile, Board &board);
    bool checkTSpin(int topLeftHeight, int topLeftWidth, Tile &currentTile, Board &board);
    bool checkBtB(bool isThisLockDifficult);
    void updateCombo(int numLineClear);
    bool isLockDifficult(int numLineClear, bool isTSpin);
    void updateIsLastLockDifficult(bool isThisLockDifficult);
    void handleSingleDropPerCell();
    void handleHardDropPerCell();
    bool isWallOrFloor(int height, int width);
};