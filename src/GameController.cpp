#include "GameController.h"
#include <cassert>
using namespace std;

const vector< pair<int,int> > GameController::TILE_POSITIONS[7][4] = {
    {
        { {1,0}, {1,1}, {1,2}, {1,3} }, // (I, 0)
        { {0,2}, {1,2}, {2,2}, {3,2} }, // (I, 1)
        { {2,3}, {2,2}, {2,1}, {2,0} }, // (I, 2)
        { {3,1}, {2,1}, {1,1}, {0,1} }, // (I, 3)
    },
    {
        { {0,0}, {1,0}, {1,1}, {1,2} }, // (J, 0)
        { {0,2}, {0,1}, {1,1}, {2,1} }, // (J, 1)
        { {2,2}, {1,2}, {1,1}, {1,0} }, // (J, 2)
        { {2,0}, {2,1}, {1,1}, {0,1} }, // (J, 3)
    },
    {
        { {0,2}, {1,2}, {1,1}, {1,0} }, // (L, 0)
        { {2,2}, {2,1}, {1,1}, {0,1} }, // (L, 1)
        { {2,0}, {1,0}, {1,1}, {1,2} }, // (L, 2)
        { {0,0}, {0,1}, {1,1}, {2,1} }, // (L, 3)
    },
    {
        { {0,1}, {0,2}, {1,1}, {1,2} }, // (O, 0)
        { {0,1}, {0,2}, {1,1}, {1,2} }, // (O, 1)
        { {0,1}, {0,2}, {1,1}, {1,2} }, // (O, 2)
        { {0,1}, {0,2}, {1,1}, {1,2} }, // (O, 3)
    },
    {
        { {0,2}, {0,1}, {1,1}, {1,0} }, // (S, 0)
        { {2,2}, {1,2}, {1,1}, {0,1} }, // (S, 1)
        { {2,0}, {2,1}, {1,1}, {1,2} }, // (S, 2)
        { {0,0}, {1,0}, {1,1}, {2,1} }, // (S, 3)
    },
    {
        { {0,1}, {1,0}, {1,1}, {1,2} }, // (T, 0)
        { {1,2}, {0,1}, {1,1}, {2,1} }, // (T, 1)
        { {2,1}, {1,2}, {1,1}, {1,0} }, // (T, 2)
        { {1,0}, {2,1}, {1,1}, {0,1} }, // (T, 3)
    },
    {
        { {0,0}, {0,1}, {1,1}, {1,2} }, // (Z, 0)
        { {0,2}, {1,2}, {1,1}, {2,1} }, // (Z, 1)
        { {2,2}, {2,1}, {1,1}, {1,0} }, // (Z, 2)
        { {2,0}, {1,0}, {1,1}, {0,1} }, // (Z, 3)
    }
};

const vector< pair<int,int> > GameController::MAP_ROTATION_TYPE = {
    {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}, {3, 0}, {0, 3}
};

const vector< pair<int,int> > GameController::JLSTZ_WALL_KICK_TESTS[8] = {
	{ { 0, 0},	{-1, 0},	{-1, 1},	{ 0,-2},	{-1,-2} },
	{ { 0, 0},	{ 1, 0},	{ 1,-1},	{ 0, 2},	{ 1, 2} },
	{ { 0, 0},	{ 1, 0},	{ 1,-1},	{ 0, 2},	{ 1, 2} },
	{ { 0, 0},	{-1, 0},	{-1, 1},	{ 0,-2},	{-1,-2} },
	{ { 0, 0},	{ 1, 0},	{ 1, 1},	{ 0,-2},	{ 1,-2} },
	{ { 0, 0},	{-1, 0},	{-1,-1},	{ 0, 2},	{-1, 2} },
	{ { 0, 0},	{-1, 0},	{-1,-1},	{ 0, 2},	{-1, 2} },
	{ { 0, 0},	{ 1, 0},	{ 1, 1},	{ 0,-2},	{ 1,-2} }
};

const vector< pair<int,int> > GameController::I_WALL_KICK_TESTS[8] = {
    { { 0, 0},	{-2, 0},	{ 1, 0},	{-2,-1},	{ 1, 2} },
	{ { 0, 0},	{ 2, 0},	{-1, 0},	{ 2, 1},	{-1,-2} },
	{ { 0, 0},	{-1, 0},	{ 2, 0},	{-1, 2},	{ 2,-1} },
	{ { 0, 0},	{ 1, 0},	{-2, 0},	{ 1,-2},	{-2, 1} },
	{ { 0, 0},	{ 2, 0},	{-1, 0},	{ 2, 1},	{-1,-2} },
	{ { 0, 0},	{-2, 0},	{ 1, 0},	{-2,-1},	{ 1, 2} },
	{ { 0, 0},	{ 1, 0},	{-2, 0},	{ 1,-2},	{-2, 1} },
	{ { 0, 0},	{-1, 0},	{ 2, 0},	{-1, 2},	{ 2,-1} }
};

shared_ptr<GameController> GameController::instance(nullptr);

shared_ptr<GameController> GameController::getInstance() {
    if (instance == nullptr) {
        instance = make_shared<GameController>();
    }
    return instance;
}

GameController::GameController()
    :rng(time(0)),
    board(),
    currentTile(),
    direction(),
    topLeftHeight(),
    topLeftWidth(),
    scoring(),
    nextTiles(),
    holdTile(),
    isHolding()
{
}

GameController::~GameController() {
}

// ==============================================================================================
// ======================================= Main functions =======================================
// ==============================================================================================

void GameController::genCurrentTile() {
    prepareNextTiles(); // In the beginning of the game, prepare 4 tiles

    currentTile = nextTiles.front(); nextTiles.pop_front(); // get the first tile of {nextTiles}
    prepareNextTiles(); // push another tile to the back of {nextTiles}
    
    assignCurrentTile();
    addCurrentTileToBoard();
}

// DROP FUNCTIONS
bool GameController::canDrop() {
    // Returns true if we can perform a single drop
    //         false otherwise
    deleteCurrentTileFromBoard();
    topLeftHeight++;
    bool can = validateCurrentTile();
    topLeftHeight--;
    addCurrentTileToBoard();
    return can;
}

void GameController::singleDrop() {
    if (!canDrop()) {
        return;
    }
    deleteCurrentTileFromBoard();
    topLeftHeight++;
    addCurrentTileToBoard();
}

void GameController::softDrop() {
    // soft drop: perform 2 single drops
    for (int turn = 0; turn < 2; ++turn) {
        if (canDrop()) {
            singleDrop();
            scoring.handleSoftDropPerCell();
        }
    }
}

void GameController::hardDrop() {
    // hard drop: perform multiple single drops until the current tile cannot be dropped anymore
    while(canDrop()) {
        singleDrop();
        scoring.handleHardDropPerCell();
    }
}

// MOVE FUNCTIONS
void GameController::moveLeft() {
    deleteCurrentTileFromBoard();
    topLeftWidth--;
    bool can = validateCurrentTile();
    if (!can) {
        topLeftWidth++;
    }
    addCurrentTileToBoard();
    scoring.isLastMoveRotate = false; // last successful movement is MOVE
}

void GameController::moveRight() {
    deleteCurrentTileFromBoard();
    topLeftWidth++;
    bool can = validateCurrentTile();
    if (!can) {
        topLeftWidth--;
    }
    addCurrentTileToBoard();
    scoring.isLastMoveRotate = false; // last successful movement is MOVE
}

// ROTATE FUNCTIONS (WITH WALL KICK)
void GameController::rotateLeft() {
    TileType currentTileType = currentTile.getType();
    if (currentTileType == O) {
        return; // rotations are not applied to O-tile
    }
    int newDirection = (direction + 3) % 4;
    int rotationID = getRotationID(direction, newDirection);

    deleteCurrentTileFromBoard();
    direction = newDirection;
    if (!wallKick(currentTileType == I ? I_WALL_KICK_TESTS[rotationID] : JLSTZ_WALL_KICK_TESTS[rotationID])) {
        // wallKick failed!
        direction = (direction + 1) % 4; // undo the rotation
    }
    addCurrentTileToBoard();

    if (direction == newDirection) {
        scoring.isLastMoveRotate = true; // last successful movement is ROTATE
    }
}

void GameController::rotateRight() {
    TileType currentTileType = currentTile.getType();
    if (currentTileType == O) {
        return; // rotations are not applied to O-tile
    }
    int newDirection = (direction + 1) % 4;
    int rotationID = getRotationID(direction, newDirection);

    deleteCurrentTileFromBoard();
    direction = newDirection;
    if (!wallKick(currentTileType == I ? I_WALL_KICK_TESTS[rotationID] : JLSTZ_WALL_KICK_TESTS[rotationID])) {
        // wallKick failed!
        direction = (direction + 3) % 4; // undo the rotation
    }
    addCurrentTileToBoard();

    if (direction == newDirection) {
        scoring.isLastMoveRotate = true; // last successful movement is ROTATE
    }
}

// HOLD FUNCTION
void GameController::hold() {
    if (isHolding) return;
    
    isHolding = true;
    if (holdTile.getType() == EMPTY) { // In the beginning of the game, holdTile does not exist
        holdTile = nextTiles.front(); nextTiles.pop_front(); // get the first tile of {nextTiles}
        prepareNextTiles(); // push another tile to the back of {nextTiles}
    }

    deleteCurrentTileFromBoard();
    swap(holdTile, currentTile);
    assignCurrentTile();
    addCurrentTileToBoard();
}

// COLLAPSE FUNCTION
int GameController::collapse() {
    // collapse() is called right after the currentTile is locked.
    // It updates the board, activates the scoring system and resets holding state, 
    // then waits for the currentTile to be generated.

    vector<int> fullTileHeightDescending = getFullTileHeightDescending();
    int numLineClear = 0;
    size_t ptr = 0;
    for (int height = Constants::BOARD_HEIGHT - 1; height >= Constants::BOARD_HEIGHT/2; --height) {
        if (ptr < fullTileHeightDescending.size() && fullTileHeightDescending[ptr] == height) {
            ++numLineClear;
            ++ptr;
            continue;
        }
        for (int width = 0; width < Constants::BOARD_WIDTH; ++width) {
            board[height + numLineClear][width] = board[height][width];
        }
    }

    // activate GameScoring
    scoring.handleScore(numLineClear, topLeftHeight, topLeftWidth, currentTile, board);

    // reset isHolding
    isHolding = false;

    return numLineClear;
}





//public get-set section
Board* GameController::getBoard() {
    return &board;
}

int GameController::getScore() {
    return scoring.score;
}






// ==============================================================================================
// ====================================== Helper functions ======================================
// ==============================================================================================
vector<int> GameController::getFullTileHeightDescending() {
    vector<int> res;
    for (int height = Constants::BOARD_HEIGHT - 1; height >= Constants::BOARD_HEIGHT/2; --height) {
        bool hasEmpty = false;
        for (int width = 0; width < Constants::BOARD_WIDTH; ++width) {
            if (board[height][width].getType() == EMPTY) {
                hasEmpty = true;
                break;
            }
        }

        if (!hasEmpty) {
            res.push_back(height);
        }
    }
    return res;
}

void GameController::assignCurrentTile() {
    direction = 0;
    topLeftHeight = 19;
    topLeftWidth = 3;
}

int GameController::getTileID(TileType tileType) {
    // Returns index of TileType c in MAP_TILE_TYPE
    for (int i = 0; i < (int) Constants::MAP_TILE_TYPE.size(); ++i) {
        if (tileType == Constants::MAP_TILE_TYPE[i]) return i;
    }
    assert(0 && "getTileID: failed!\n");
} 

bool GameController::positionInsideBoard(int height, int width) {
    return height >= 0 && height < Constants::BOARD_HEIGHT && width >= 0 && width < Constants::BOARD_WIDTH;
}

vector < pair<int,int> > GameController::getCurrentTilePositions() {
    int tileID = getTileID(currentTile.getType());
    vector < pair<int,int> > res;
    for (int i = 0; i < 4; ++i) {
        int height = topLeftHeight + TILE_POSITIONS[tileID][direction][i].first;
        int width = topLeftWidth + TILE_POSITIONS[tileID][direction][i].second;
        res.push_back({height, width});
    }
    return res;
}

void GameController::deleteCurrentTileFromBoard() {
    TileType currentTileType = currentTile.getType();
    vector < pair<int,int> > currentTilePositions = getCurrentTilePositions();
    for (auto &position : currentTilePositions) {
        int height = position.first, width = position.second;
        if (!positionInsideBoard(height, width)) {
            assert(0 && "deleteCurrentTileFromBoard: position is not inside the board!\n");
        }
        if (board[height][width].getType() != currentTileType) {
            assert(0 && "deleteCurrentTileFromBoard: conflict TileType.\n");
        }
        board[height][width] = Tile(EMPTY);
    }
}

void GameController::addCurrentTileToBoard() {
    TileType currentTileType = currentTile.getType();
    vector < pair<int,int> > currentTilePositions = getCurrentTilePositions();
    for (auto &position : currentTilePositions) {
        int height = position.first, width = position.second;
        if (!positionInsideBoard(height, width)) {
            assert(0 && "addCurrentTileToBoard: position is not inside the board!\n");
        }
        if (board[height][width].getType() != EMPTY) {
            assert(0 && "addCurrentTileToBoard: cell is not empty to fill.\n");
        }
        board[height][width] = Tile(currentTileType);
    }
}

bool GameController::validateCurrentTile() {
    // validateCurrentTile() checks if it is possible to put the current tile in this position
    // because one of the tile positions is not empty or out of board
    // Returns true if possible
    //         false otherwise
    vector < pair<int,int> > currentTilePositions = getCurrentTilePositions();
    for (auto &position : currentTilePositions) {
        int height = position.first, width = position.second;
        if (!positionInsideBoard(height, width)) {
            return false; // out of board (do not assert!)
        }
        if (board[height][width].getType() != EMPTY) {
            return false; // not empty
        }
    }
    return true;
}

int GameController::getRotationID(int beforeDirection, int afterDirection) {
    for (int i = 0; i < (int) MAP_ROTATION_TYPE.size(); ++i) {
        if (MAP_ROTATION_TYPE[i] == make_pair(beforeDirection, afterDirection)) {
            return i;
        }
    }
    assert(0 && "getRotationID: failed!\n");
}

bool GameController::wallKick(vector < pair<int,int> > tests) {
    // Returns false if all the tests fail
    //         true if one of the tests works, and change the topLeftHeight and topLeftWidth as well
    for (auto test: tests) {
        topLeftHeight -= test.second;
        topLeftWidth += test.first;
        if (!validateCurrentTile()) {
            // undo the test
            topLeftHeight += test.second;
            topLeftWidth -= test.first;
        }
        else {
            return true;
        }
    }
    return false;
}

void GameController::prepareNextTiles() {
    while(nextTiles.size() < 4) {
        while(true) {
            int randNum = rng() % 7;
            Tile tile = Tile(Constants::MAP_TILE_TYPE[randNum]);
            if (nextTiles.empty() || nextTiles.back() != tile) {
                nextTiles.push_back(tile);
                break;
            }
        }
    }
}