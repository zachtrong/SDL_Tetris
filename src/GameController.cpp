#include "GameController.h"

GameController::GameController() {
}

void GameController::genNewTile() {
    int randNum = rand() % 7;
    currentTile = Tile(Constants::MAP_TILE_TYPE[randNum]);
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

// utils
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