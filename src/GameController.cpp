#include "GameController.h"
#include "Constants.h"
#include "Tile.h"
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <cassert>
using namespace std;

GameController::GameController() {
}

// ==============================================================================================
// ======================================= Main functions =======================================
// ==============================================================================================

void GameController::genCurrentTile() {
    mt19937 rng(time(0));
    int randNum = rng() % 7;
    currentTile = Tile(Constants::MAP_TILE_TYPE[randNum]);
    direction = 0;
    assignCurrentTile();
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
    singleDrop();
    singleDrop();
}

void GameController::hardDrop() {
    // hard drop: perform single drops until the current tile cannot be dropped anymore
    while(canDrop()) {
        singleDrop();
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
}

void GameController::moveRight() {
    deleteCurrentTileFromBoard();
    topLeftWidth++;
    bool can = validateCurrentTile();
    if (!can) {
        topLeftWidth--;
    }
    addCurrentTileToBoard();
}

// ROTATE FUNCTIONS (WITH WALL KICKS)
void GameController::rotateLeft() {
    direction = (direction + 3) % 4;
}

void GameController::rotateRight() {
    direction = (direction + 1) % 4;
}

// COLLAPSE FUNCTION
void GameController::collapse() {
    vector<int> fullTileHeightDescending = getFullTileHeightDescending();

    int numDeleted = 0;
    int ptr = 0;
    for (int height = Constants::BOARD_HEIGHT - 1; height >= 0; --height) {
        if (ptr < (int) fullTileHeightDescending.size() && fullTileHeightDescending[ptr] == height) {
            ++numDeleted;
            continue;
        }
        for (int width = 0; width < Constants::BOARD_WIDTH; ++width) {
            board[height][width] = board[height - numDeleted][width];
        }
    }
}








// ==============================================================================================
// ====================================== Helper functions ======================================
// ==============================================================================================
vector<int> GameController::getFullTileHeightDescending() {
    vector<int> res;
    for (int height = Constants::BOARD_HEIGHT - 1; height >= 0; --height) {
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
    topLeftHeight = 18;
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
        int height = topLeftHeight + Constants::TILE_POSITIONS[tileID][direction][i].first;
        int width = topLeftWidth + Constants::TILE_POSITIONS[tileID][direction][i].second;
        res.push_back(make_pair(height, width));
    }
    return res;
}

void GameController::deleteCurrentTileFromBoard() {
    vector < pair<int,int> > currentTilePositions = getCurrentTilePositions();
    for (auto &position : currentTilePositions) {
        int height = position.first, width = position.second;
        if (!positionInsideBoard(height, width)) {
            assert(0 && "deleteCurrentTileFromBoard: position is not inside the board!\n");
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
    // either because one of the cells is not empty or because one of the tile positions is out of board
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