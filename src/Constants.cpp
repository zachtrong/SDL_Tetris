#include "Constants.h"

Constants::Constants() {
}

Constants::~Constants() {

}

const int Constants::SCREEN_WIDTH = 400;
const int Constants::SCREEN_HEIGHT = 680;
const int Constants::TILE_SIZE = 40;
const int Constants::BOARD_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int Constants::BOARD_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

const vector<TileType> Constants::MAP_TILE_TYPE = {I, J, L, O, S, T, Z}; 

const char* const Constants::GAME_TITLE = "Tetris";
const char* const Constants::BACKGROUND_IMG_PATH = "assets/textures/background.png";
