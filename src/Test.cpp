#include "Test.h"
#include <string>
using namespace std;

string Test::testName = "";

Test::Test() {
}

Test::~Test() {

}

void Test::assert(bool b) {
    if (!b) {
        throw testName + " fail";
    }
}

template <typename T>
void Test::assertEqual(T a, T b) {
    if (a != b) {
        throw testName + " fail";
    }
}

void Test::setTestName(string t) {
    testName = t;
}

void Test::finishTest() {
    printf("%s ok\n", testName.c_str());
}

void Test::runAllTest() {
    try {
        setTestName("Test Board.cpp");
        Test::runTestBoard();
        finishTest();

        setTestName("Test Tile.cpp");
        Test::runTestTile();
        finishTest();
    } catch (const char *message) {
        printf("%s\n", message);
    }
}

void Test::runTestBoard() {
    Board b;
    b[1][2] = 3;
    assertEqual(b[1][2], 3);
}

void Test::runTestTile() {
    Tile *tile = new Tile(I);
    assertEqual(tile->getAssetPath(), string("assets/textures/1x/tile_Imdpi.png"));
    delete tile;
    tile = new Tile(J);
    assertEqual(tile->getAssetPath(), string("assets/textures/1x/tile_Jmdpi.png"));
    delete tile;
}