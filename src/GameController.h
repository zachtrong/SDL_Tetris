#pragma once
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include "Constants.h"
#include "Board.h"
using namespace std;

class GameController {
private:

public:
    Board board;
    Tile currentTile;
    int direction; // {0, 1, 2, 3}

    GameController();
    
    void collapse();
    vector<int> getFullTileHeightDescending();

    void genCurrentTile();
    void assignCurrentTilePosition();

    void softDrop();
    void hardDrop();
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();
};