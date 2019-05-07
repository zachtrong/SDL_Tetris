#pragma once
#include <memory>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <exception>
#include <vector>
#include <mutex>
#include <map>
#include <string>
#include <algorithm>
#include "Tile.h"

using namespace std;

struct DisplayObject {
    string path;
    SDL_Rect rect;
    DisplayObject(string _path, const SDL_Rect &_rect): path(_path), rect(_rect) {}
};

struct RectObject {
	SDL_Rect rect;
	SDL_Color color;
	RectObject(const SDL_Rect &_rect, const SDL_Color &_color): rect(_rect), color(_color) {}
};

struct FontObject {
	string text; 
	int fontSize; 
	SDL_Rect rect; 
	bool isBold;
	FontObject(string _text, const SDL_Rect &_rect): text(_text), fontSize(24), rect(_rect), isBold(false) {} 
	FontObject(string _text, int _fontSize, const SDL_Rect &_rect): 
		text(_text), fontSize(_fontSize), rect(_rect), isBold(false) {} 
	FontObject(string _text, int _fontSize, const SDL_Rect &_rect, bool _isBold): 
		text(_text), fontSize(_fontSize), rect(_rect), isBold(_isBold) {} 
};

struct TileObject {
	TileType type;
	SDL_Rect rect;
	TileObject(TileType _type, const SDL_Rect &_rect): type(_type), rect(_rect) {}
};

struct FullTileObject {
	TileType type;
	SDL_Rect rect;
	FullTileObject(TileType _type, const SDL_Rect &_rect): type(_type), rect(_rect) {}
};