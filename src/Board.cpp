#include "Board.h"

BoxArray::BoxArray()
    :state(new int[Constants::BOARD_HEIGHT]) {
    for (int i = 0; i < Constants::BOARD_HEIGHT; ++i) {
        state[i] = 0;
    }
}

BoxArray::~BoxArray() {
    delete[] state;
}

int& BoxArray::operator[](int x) {
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

void test() {
    Board b;
    b[1][2] = 3;
    cout << b[1][2] << endl;
}