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
	map<TileType, shared_ptr<SDL_Texture> > tileTextures;

	shared_ptr<TTF_Font> scoringFont;
	SDL_Color colorWhite;
	SDL_Color colorBackground;

	void init();
	void initWindow();
	void initRenderer();
	void initImage();
	void initTileTexture();
	void initTextureText();

	void drawBackground();
	void drawLinesOnBackground();

	void drawTextureText();
	void drawTextureHold();
	void drawTextureNext();
	void drawTextureScore();
	void drawTextureFooter();

	shared_ptr<SDL_Texture> createTextureText(const string &text, int fontSize, SDL_Rect *rect);
	shared_ptr<SDL_Texture> createTextureTextScoring(const string &text, SDL_Rect *rect);
	shared_ptr<SDL_Texture> createTexture(const string &path);
	shared_ptr<SDL_Surface> createSurface(const string &path);
	void copyTileToRenderer(Tile *t);
public:
    static shared_ptr<GameView> getInstance();
    GameView();
    virtual ~GameView();

	void startSDL();
    void updateBoard(Board &board);
	void updateScore(int score);
};