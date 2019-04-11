#pragma once
#include <string>
using namespace std;

/*
    Reference:
    https://tetris.fandom.com/wiki/Tetromino
    tile X equal NULL
*/

enum TileType {
    EMPTY,
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
public:
    Tile(TileType type = EMPTY);
    string getAssetPath();
    TileType getType() const;
    virtual ~Tile();

    friend bool operator==(const Tile &lhs, const Tile &rhs) {
        return lhs.getType() == rhs.getType();
    }

    friend bool operator!=(const Tile &lhs, const Tile &rhs) {
        return lhs.getType() != rhs.getType();
    }
};