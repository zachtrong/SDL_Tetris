#pragma once

class Tile {
private:
    enum Type {
        I, J, L, O, S, T, Z
    };
public:
    Tile();
    virtual ~Tile();
};