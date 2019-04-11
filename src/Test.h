#include "Game.h"
#include "Board.h"
#include "Tile.h"
#include <string>
#pragma once

using namespace std;

class Test {
private:
    static string testCase;
    static void assert(bool b);
    template <typename T>
    static void assertEqual(T a, T b);
    template <typename T>
    static void assertNotEqual(T a, T b);
public:
    Test();
    virtual ~Test();
    static void setTestCase(string testName);
    static void finishTestCase();
    static void runAllTest();
    static void printTestName(string testName);
    static string failedTestCase();

    static void runTestBoard();
    static void runTestTile();
};