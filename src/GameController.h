#pragma once
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

class GameController {
private:
    static const int *shape;

public:
    static const int WIDTH = 10;
    static const int HEIGHT = 40;
    
    int color[WIDTH][HEIGHT];

    GameController();
    
    void collapse();
    void softDrop();
    void hardDrop();
    void newTetris();
    void moveLeft();
    void moveRight();
    void rotateLeft();
    void rotateRight();
};