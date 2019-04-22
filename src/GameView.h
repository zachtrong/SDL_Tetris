#pragma once
#include <memory>
#include <map>
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <exception>
#include <vector>
#include <mutex>
#include <map>
#include "Exception.h"
#include "Constants.h"
#include "PointerDefinition.h"
#include "Board.h"
using namespace std;

/*
    usage GameView::getInstance()->updateBoard(const Board &board);
*/
class GameView {
private:
    static shared_ptr<GameView> instance;
	shared_ptr<SDL_Window> window;
	shared_ptr<SDL_Surface> windowSurface;
	shared_ptr<SDL_Renderer> renderer;
	shared_ptr<SDL_Texture> texture;
	map<TileType, shared_ptr<SDL_Texture> > tileTextures;
public:
    static shared_ptr<GameView> getInstance();
    GameView();
    virtual ~GameView();

	void startSDL();
	void drawBackground();
	void init();
	void initWindow();
	void initRenderer();
	void initImage();
	void initTexture();
	void initTileTexture();

	shared_ptr<SDL_Texture> createTexture(string path);
	shared_ptr<SDL_Surface> createSurface(string path);
    void updateBoard(Board &board);
	void copyTileToRenderer(Tile *t);
};