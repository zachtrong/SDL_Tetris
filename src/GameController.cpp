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
    assignCurrentTilePosition();
}

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

void GameController::assignCurrentTilePosition() {
    
}

int getTileID(TileType &tileType) {
    // Returns index of TileType c in MAP_TILE_TYPE
    for (int i = 0; i < (int) Constants::MAP_TILE_TYPE.size(); ++i) {
        if (tileType == Constants::MAP_TILE_TYPE[i]) return i;
    }
    assert(0 && "getTileID: failed!\n");
} 

bool GameController::positionInsideBoard(int height, int width) {
    return height >= 0 && height < Constants::BOARD_HEIGHT && width >= 0 && width < Constants::BOARD_WIDTH;
}

void GameController::deleteCurrentTileFromBoard() {
    int tileID = getTileID(currentTile.getType());
    for (int i = 0; i < 4; ++i) {
        int curHeight = topLeftHeight + Constants::TILE_POSITIONS[tileID][direction][i].first;
        int curWidth = topLeftWidth + Constants::TILE_POSITIONS[tileID][direction][i].second;
        if (!positionInsideBoard(curHeight, curWidth)) {
            assert(0 && "deleteCurrentTileFromBoard: position is not inside the board!\n");
        }
        board[curHeight][curWidth] = Tile(EMPTY);
    }
}

void GameController::addCurrentTileToBoard() {
    TileType currentTileType = currentTile.getType();
    int tileID = getTileID(currentTileType);
    for (int i = 0; i < 4; ++i) {
        int curHeight = topLeftHeight + Constants::TILE_POSITIONS[tileID][direction][i].first;
        int curWidth = topLeftWidth + Constants::TILE_POSITIONS[tileID][direction][i].second;
        if (!positionInsideBoard(curHeight, curWidth)) {
            assert(0 && "addCurrentTileToBoard: position is not inside the board!\n");
        }
        if (board[curHeight][curWidth].getType() != EMPTY) {
            assert(0 && "addCurrentTileToBoard: cell is not empty to fill");
        }
        board[curHeight][curWidth] = Tile(currentTileType);
    }
}

bool GameController::validateCurrentTile() {
    // validateCurrentTile() checks if it is possible to put the current tile in this position
    // Returns true if all cells in currentTile is empty
    //         false otherwise
    int tileID = getTileID(currentTile.getType());
    for (int i = 0; i < 4; ++i) {
        int curHeight = topLeftHeight + Constants::TILE_POSITIONS[tileID][direction][i].first;
        int curWidth = topLeftWidth + Constants::TILE_POSITIONS[tileID][direction][i].second;
        if (!positionInsideBoard(curHeight, curWidth)) {
            assert(0 && "validateCurrentTile: position is not inside the board!\n");
        }
        if (board[curHeight][curWidth].getType() != EMPTY) {
            return false; // not empty
        }
    }
    return true;
}