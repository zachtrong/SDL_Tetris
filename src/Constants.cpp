#include "Constants.h"

Constants::Constants() {
}

Constants::~Constants() {

}

const int Constants::TILE_SIZE = 30;
const int Constants::BOARD_WIDTH = 10;
const int Constants::BOARD_HEIGHT = 40;
const int Constants::SCREEN_WIDTH = TILE_SIZE * BOARD_WIDTH;
const int Constants::SCREEN_HEIGHT = TILE_SIZE * (BOARD_HEIGHT / 2);

const vector<TileType> Constants::MAP_TILE_TYPE = {I, J, L, O, S, T, Z}; 

const char* const Constants::GAME_TITLE = "Tetris";
const char* const Constants::BACKGROUND_IMG_PATH = "assets/textures/background.png";
