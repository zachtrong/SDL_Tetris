#pragma once
#include <iostream>
#include <cstring>
#include <algorithm>
#include <memory>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "Constants.h"
#include "Board.h"
#include "Tile.h"
using namespace std;

class GameController {
private:
    static shared_ptr<GameController> instance;
    static const vector< pair<int,int> > TILE_POSITIONS[7][4];
		// --> tile positions of 7 Tiles with 4 directions (see the docs)

    static const vector < pair<int,int> > MAP_ROTATION_TYPE;
    
    static const vector < pair<int,int> > JLSTZ_WALL_KICK_TESTS[8];
    static const vector < pair<int,int> > I_WALL_KICK_TESTS[8];
		// --> wall kick tests, map->first is the change of direction, map->second is a vector of tests (see the docs)
public:
    static shared_ptr<GameController> getInstance();
    mt19937 rng;
    Board board;
    Tile currentTile;
    int direction; // {0, 1, 2, 3}
    int topLeftHeight;
    int topLeftWidth;

    GameController();
    virtual ~GameController();

    // Main functions
    int collapse();
    void genCurrentTile();
    bool canDrop();
    void singleDrop();
    void softDrop();
    void hardDrop();
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();

    //get-set
    Board* getBoard();

    // Helper functions
    int getTileID(TileType tileType);
    void assignCurrentTile();
    vector<int> getFullTileHeightDescending();
    bool positionInsideBoard(int height, int width);
    vector< pair<int,int> > getCurrentTilePositions();
    void deleteCurrentTileFromBoard();
    void addCurrentTileToBoard();
    bool validateCurrentTile();
    int getRotationID(int beforeDirection, int afterDirection);
    bool wallKick(vector < pair<int,int> > tests);
};