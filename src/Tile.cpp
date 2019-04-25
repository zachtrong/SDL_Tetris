#include "Tile.h"

const string Tile::FILE_PREFIX = "assets/textures/2x/tile_";
const string Tile::FILE_SUFFIX = "xhdpi.png";

Tile::Tile(TileType type) {
    this->type = type;
}

Tile::~Tile() {

}

string Tile::getAssetPath() const {
    if (type == EMPTY) {
        return FILE_PREFIX + "EMPTY" + FILE_SUFFIX;
    }
    char c = type;
    return FILE_PREFIX + c + FILE_SUFFIX;
}

string Tile::getAssetFullTilePath() const {
    if (type == EMPTY) {
        return FILE_PREFIX + "EMPTY_full" + FILE_SUFFIX;
    }
    char c = type;
    return FILE_PREFIX + c + "_full" + FILE_SUFFIX;
}

TileType Tile::getType() const {
    return type;
}

void Tile::setType(const TileType &t) {
    this->type = t;
}

void Tile::setPositionOnWindow(int x, int y, int w, int h) {
    positionOnWindow.x = x;
    positionOnWindow.y = y;
    positionOnWindow.w = w;
    positionOnWindow.h = h;
}

SDL_Rect& Tile::getPositionOnWindow() {
    return positionOnWindow;
}