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


const vector<TileType> MAP_TILE_TYPE = {I, J, L, O, S, T, Z}; 

const vector< pair<int,int> > TILE_POSITIONS[7][4] = {
    {
        { make_pair(1,0), make_pair(1,1), make_pair(1,2), make_pair(1,3) }, // (I, 0)
        { make_pair(0,2), make_pair(1,2), make_pair(2,2), make_pair(3,2) }, // (I, 1)
        { make_pair(2,3), make_pair(2,2), make_pair(2,1), make_pair(2,0) }, // (I, 2)
        { make_pair(3,1), make_pair(2,1), make_pair(1,1), make_pair(0,1) }, // (I, 3)
    },
    {
        { make_pair(0,0), make_pair(1,0), make_pair(1,1), make_pair(1,2) }, // (J, 0)
        { make_pair(0,2), make_pair(0,1), make_pair(1,1), make_pair(2,1) }, // (J, 1)
        { make_pair(2,2), make_pair(1,2), make_pair(1,1), make_pair(1,0) }, // (J, 2)
        { make_pair(2,0), make_pair(2,1), make_pair(1,1), make_pair(0,1) }, // (J, 3)
    },
    {
        { make_pair(0,2), make_pair(1,2), make_pair(1,1), make_pair(1,0) }, // (L, 0)
        { make_pair(2,2), make_pair(2,1), make_pair(1,1), make_pair(0,1) }, // (L, 1)
        { make_pair(2,0), make_pair(1,0), make_pair(1,1), make_pair(1,2) }, // (L, 2)
        { make_pair(0,0), make_pair(0,1), make_pair(1,1), make_pair(2,1) }, // (L, 3)
    },
    {
        { make_pair(0,1), make_pair(0,2), make_pair(1,1), make_pair(1,2) }, // (O, 0)
        { make_pair(0,1), make_pair(0,2), make_pair(1,1), make_pair(1,2) }, // (O, 1)
        { make_pair(0,1), make_pair(0,2), make_pair(1,1), make_pair(1,2) }, // (O, 2)
        { make_pair(0,1), make_pair(0,2), make_pair(1,1), make_pair(1,2) }, // (O, 3)
    },
    {
        { make_pair(0,2), make_pair(0,1), make_pair(1,1), make_pair(1,0) }, // (S, 0)
        { make_pair(2,2), make_pair(1,2), make_pair(1,1), make_pair(0,1) }, // (S, 1)
        { make_pair(2,0), make_pair(2,1), make_pair(1,1), make_pair(1,2) }, // (S, 2)
        { make_pair(0,0), make_pair(1,0), make_pair(1,1), make_pair(2,1) }, // (S, 3)
    },
    {
        { make_pair(0,1), make_pair(1,0), make_pair(1,1), make_pair(1,2) }, // (T, 0)
        { make_pair(1,2), make_pair(0,1), make_pair(1,1), make_pair(2,1) }, // (T, 1)
        { make_pair(2,1), make_pair(1,2), make_pair(1,1), make_pair(1,0) }, // (T, 2)
        { make_pair(1,0), make_pair(2,1), make_pair(1,1), make_pair(0,1) }, // (T, 3)
    },
    {
        { make_pair(0,0), make_pair(0,1), make_pair(1,1), make_pair(1,2) }, // (Z, 0)
        { make_pair(0,2), make_pair(1,2), make_pair(1,1), make_pair(2,1) }, // (Z, 1)
        { make_pair(2,2), make_pair(2,1), make_pair(1,1), make_pair(1,0) }, // (Z, 2)
        { make_pair(2,0), make_pair(1,0), make_pair(1,1), make_pair(0,1) }, // (Z, 3)
    }
};

const char* const Constants::GAME_TITLE = "Tetris";
const char* const Constants::BACKGROUND_IMG_PATH = "assets/textures/background.png";
