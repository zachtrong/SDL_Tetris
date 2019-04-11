#include "Game.h"
#include "Constants.h"
#include "Tile.h"
#pragma once

/*
    class Board represent the color and state of Tetris game
    usage: board[width][height]
*/

class BoxArray {
private:
    Tile *state;
public:
    BoxArray();
    virtual ~BoxArray();

    Tile& operator[](int x);
};

class Board {
private:
    BoxArray *state;
public:
    Board();
    virtual ~Board();

    BoxArray& operator[](int x); 
};