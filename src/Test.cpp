#include "Test.h"
#include <string>
using namespace std;

string Test::testCase = "";

Test::Test() {
}

Test::~Test() {

}

void Test::assert(bool b) {
    if (!b) {
        throw failedTestCase();
    }
}

template <typename T>
void Test::assertEqual(T a, T b) {
    if (!(a == b)) {
        throw failedTestCase();
    }
}

template <typename T>
void Test::assertNotEqual(T a, T b) {
    if (!(a != b)) {
        throw failedTestCase();
    }
}

string Test::failedTestCase() {
    return "\tFAILED: " + testCase;
}

void Test::setTestCase(string t) {
    testCase = t;
}

void Test::finishTestCase() {
    printf("\tOK: %s\n", testCase.c_str());
}

void Test::printTestName(string testName) {
    printf("Start testing: %s\n", testName.c_str());
}

void Test::runAllTest() {
    try {
        printTestName("Board.cpp");
        Test::runTestBoard();

        printTestName("Tile.cpp");
        Test::runTestTile();
    } catch (const char *message) {
        printf("%s\n", message);
    }
}

void Test::runTestBoard() {
    Board b;

    setTestCase("default value");
    assertEqual(b[1][1], Tile());
    assertEqual(b[1][1], Tile(EMPTY));
    assertEqual(b[0][1], Tile(EMPTY));
    assertEqual(b[0][1], Tile());
    finishTestCase();

    setTestCase("set new value");
    b[1][2] = Tile(T);
    assertEqual(b[1][2], Tile(T));
    b[1][2] = Tile();
    assertEqual(b[1][2], Tile());
    finishTestCase();
}

void Test::runTestTile() {
    shared_ptr<Tile> tile;

    setTestCase("arbitrary value");
    tile = make_shared<Tile>(I);
    assertEqual(tile->getAssetPath(), string("assets/textures/1x/tile_Imdpi.png"));
    tile = make_shared<Tile>(J);
    assertEqual(tile->getAssetPath(), string("assets/textures/1x/tile_Jmdpi.png"));
    finishTestCase();

    setTestCase("default value");
    tile = make_shared<Tile>();
    assertEqual(tile->getAssetPath(), string("assets/textures/1x/tile_EMPTYmdpi.png"));
    tile = make_shared<Tile>(EMPTY);
    assertEqual(tile->getAssetPath(), string("assets/textures/1x/tile_EMPTYmdpi.png"));
    finishTestCase();
}