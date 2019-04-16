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

Board::Board()
    :state(new BoxArray[Constants::BOARD_HEIGHT]) {
    
    for (int i = 0; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
            this->operator[](i).operator[](j).setPositionOnWindow(
                Constants::TILE_SIZE * i,
                Constants::TILE_SIZE * j,
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