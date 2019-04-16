#include "GameController.h"
#include "Constants.h"
#include "Tile.h"
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
using namespace std;

GameController::GameController() {
}

// ============ Main functions ============
void GameController::genCurrentTile() {
    mt19937 rng(time(0));
    int randNum = rng() % 7;
    currentTile = Tile(Constants::MAP_TILE_TYPE[randNum]);
    direction = 0;
    assignCurrentTilePosition();
}

void GameController::collapse() {
    vector<int> fullTileHeightDescending = getFullTileHeightDescending();

    int numDeleted = 0;
    int ptr = 0;
    for (int height = Constants::BOARD_HEIGHT - 1; height >= 0; --height) {
        if (ptr < fullTileHeightDescending.size() && fullTileHeightDescending[ptr] == height) {
            ++numDeleted;
            continue;
        }
        for (int width = 0; width < Constants::BOARD_WIDTH; ++width) {
            board[height][width] = board[height - numDeleted][width];
        }
    }
}

// ============ Helper functions ============
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