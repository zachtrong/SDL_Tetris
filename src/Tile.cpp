#include "Tile.h"

const string Tile::FILE_PREFIX = "assets/textures/1x/tile_";
const string Tile::FILE_SUFFIX = "mdpi.png";

Tile::Tile() {

}

Tile::Tile(TileType type) {
    this->type = type;
}

Tile::~Tile() {

}

string Tile::getAssetPath() {
    return FILE_PREFIX + char(type) + FILE_SUFFIX;
}