#pragma once
#include "Tile.h"
#include <vector>
using namespace std;

class Constants {
private:
	Constants();
	virtual ~Constants();
public:
	static const int SCREEN_WIDTH;
	static const int SCREEN_HEIGHT;
	static const int TILE_SIZE;
    static const int BOARD_WIDTH;
    static const int BOARD_HEIGHT;
	static const vector<TileType> MAP_TILE_TYPE;

	static const char* const GAME_TITLE;
	static const char* const BACKGROUND_IMG_PATH;
};