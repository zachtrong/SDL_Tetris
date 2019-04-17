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
    int topLeftHeight;
    int topLeftWidth;

    GameController();
    
    // Main functions
    void collapse();
    void genCurrentTile();
    bool canDrop();
    void singleDrop();
    void softDrop();
    void hardDrop();
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();

    // Helper functions
    int getTileID();
    void assignCurrentTilePosition();
    vector<int> getFullTileHeightDescending();
    bool positionInsideBoard(int height, int width);
    void deleteCurrentTileFromBoard();
    void addCurrentTileToBoard();
    bool validateCurrentTile();
};