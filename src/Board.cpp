#include "Board.h"

BoxArray::BoxArray()
    :state(new Tile[Constants::BOARD_WIDTH]) {
}

BoxArray::~BoxArray() {
    delete[] state;
}

Tile& BoxArray::operator[](int x) {
    return state[x];
}

//[20-39][0-9]
Board::Board()
    :state(new BoxArray[Constants::BOARD_HEIGHT]) {
    
    for (int i = Constants::BOARD_HEIGHT/2; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
            this->operator[](i).operator[](j).setPositionOnWindow(
                Constants::SCREEN_OFFSET + Constants::TILE_SIZE * j,
                Constants::TILE_SIZE * (i - Constants::BOARD_HEIGHT/2),
                Constants::TILE_SIZE,
                Constants::TILE_SIZE
            );
        }
    }
};

Board::~Board() {
    delete[] state;
}

BoxArray& Board::operator[](int x) {
    return state[x];
}