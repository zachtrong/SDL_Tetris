#include "Board.h"

BoxArray::BoxArray()
    :state(new Tile[Constants::BOARD_HEIGHT]) {
}

BoxArray::~BoxArray() {
    delete[] state;
}

Tile& BoxArray::operator[](int x) {
    return state[x];
}

Board::Board()
    :state(new BoxArray[Constants::BOARD_WIDTH]) {
};

Board::~Board() {
    delete[] state;
}

BoxArray& Board::operator[](int x) {
    return state[x];
}