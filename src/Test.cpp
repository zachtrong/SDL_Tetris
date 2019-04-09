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
        Test::runTestBoard();
    } catch (const char *message) {
        printf("%s\n", message);
    }
}

void Test::runTestBoard() {
    setTestName("test board");
    Board b;
    b[1][2] = 3;
    assertEqual(b[1][2], 3);
    finishTest();
}