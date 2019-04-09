#include "Board.h"

Board::Board(): state(new int* [Constants::BOARD_WIDTH]) {
    for (int i = 0; i < Constants::BOARD_WIDTH; ++i) {
        state[i] = new int[Constants::BOARD_HEIGHT];
    }
};

Board::~Board() {
    for (int i = 0; i < Constants::BOARD_WIDTH; ++i) {
        delete[] state[i];
    }
    delete[] state;
}

