#include "Game.h"
#include "Board.h"
#include <string>
#pragma once

using namespace std;

class Test {
private:
    static string testName;
    static void assert(bool b);
    template <typename T>
    static void assertEqual(T a, T b);
public:
    Test();
    virtual ~Test();
    static void setTestName(string testName);
    static void finishTest();
    static void runAllTest();

    static void runTestBoard();
};