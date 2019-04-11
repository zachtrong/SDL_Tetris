#pragma once
#include <string>
using namespace std;

/*
    Reference:
    https://tetris.fandom.com/wiki/Tetromino
*/

enum TileType {
    I = 'I', 
    J = 'J', 
    L = 'L', 
    O = 'O', 
    S = 'S', 
    T = 'T', 
    Z = 'Z'
};

class Tile {
private:
    static const string FILE_PREFIX;
    static const string FILE_SUFFIX;
    TileType type;
    Tile();
public:
    Tile(TileType type);
    string getAssetPath();
    virtual ~Tile();
};