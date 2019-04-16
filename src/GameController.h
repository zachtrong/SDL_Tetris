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

    GameController();
    
    void collapse();
    vector<int> getFullTileHeightDescending();

    void genNewTile();
    void assignCurrentTilePosition();

    void softDrop();
    void hardDrop();
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();
};