#include "GameController.h"

GameController::GameController() {
    memset(color, 0, sizeof color);
}

void GameController::collapse() {
    while(true) {
        // check if the last row contains all colored cells
        bool allColored = true;
        for (int col = 0; col < WIDTH; ++col) {
            if (color[HEIGHT-1][col] == 0) {
                allColored = false;
                break;
            }
        }
        if (!allColored) {
            break;
        }
        // destroy the last row
        for (int row = HEIGHT-1; row >= 1; --row) {
            for (int col = 0; col < WIDTH; ++col) {
                color[row][col] = color[row - 1][col];
            }
        }
        for (int col = 0; col < WIDTH; ++col) {
            color[0][col] = 0; // uncolored
        }
    }
}