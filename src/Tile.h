#pragma once
#include <string>
#include "SDL.h"
using namespace std;

/*
    Reference:
    https://tetris.fandom.com/wiki/Tetromino
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
    SDL_Rect positionOnWindow;
public:
    Tile(TileType type = EMPTY);
    string getAssetPath() const;
    TileType getType() const;
    virtual ~Tile();

    void setPositionOnWindow(int x, int y, int w, int h);
    SDL_Rect& getPositionOnWindow();

    Tile& operator=(const Tile& other) {
        if (&other == this) {
            return *this;
        }
        this->type = other.type;
        return *this;
    }

    friend bool operator==(const Tile &lhs, const Tile &rhs) {
        return lhs.getType() == rhs.getType();
    }

    friend bool operator!=(const Tile &lhs, const Tile &rhs) {
        return lhs.getType() != rhs.getType();
    }
};