#include "Game.h"
#include "Constants.h"
#pragma once

/*
    class Board represent the color and state of Tetris game
    usage: board[width][height]
*/

class BoxArray {
private:
    int *state;
public:
    BoxArray();
    virtual ~BoxArray();

    int& operator[](int x);
};

class Board {
private:
    BoxArray *state;
public:
    Board();
    virtual ~Board();

    BoxArray& operator[](int x); 
};