#include "Game.h"
#include "Constants.h"
#pragma once

/*
    class Board represent the color and state of Tetris game
    usage: board[width][height]
*/
class Board {
private:
    int **state;
public:
    Board();
    virtual ~Board();
};