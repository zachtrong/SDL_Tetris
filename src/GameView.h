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
	static const SDL_Rect RECT_BACKGROUND;
	static const SDL_Rect RECT_BORDER_LEFT;
	static const SDL_Rect RECT_BORDER_RIGHT;
	static const SDL_Rect RECT_BACKGROUND_SCORE;
	static const SDL_Rect RECT_HOLDING;
	static const SDL_Rect RECT_PREPARING[4];

	shared_ptr<SDL_Window> window;
	shared_ptr<SDL_Surface> windowSurface;
	shared_ptr<SDL_Renderer> renderer;
	map<TileType, shared_ptr<SDL_Texture> > tileTextures;
	map<TileType, shared_ptr<SDL_Texture> > fullTileTextures;

	shared_ptr<TTF_Font> scoringFont;
	SDL_Color colorWhite;
	SDL_Color colorBackground;

	void init();
	void initWindow();
	void initRenderer();
	void initImage();
	void initTileTexture();
	void initFullTileTexture();
	void initTextureText();

	void drawBackground();
	void drawSceneStartBackground();
	void drawLinesOnBackground();

	void drawTextureText();
	void drawTextureHold();
	void drawTextureNext();
	void drawTextureScore();
	void drawTextureFooter();

	shared_ptr<SDL_Texture> createTextureText(const string &text, int fontSize, SDL_Rect *rect);
	shared_ptr<SDL_Texture> createTextureTextScoring(const string &text, SDL_Rect *rect);
	shared_ptr<SDL_Texture> createTextureTransparent(const string &path);
	shared_ptr<SDL_Texture> createTexture(const string &path);
	shared_ptr<SDL_Surface> createSurface(const string &path);
	void copyTileToRenderer(Tile *tile);
	void copyFullTileToRenderer(Tile *tile, const SDL_Rect &rect);
public:
    static shared_ptr<GameView> getInstance();
    GameView();
    virtual ~GameView();

	void startSDL();

	void drawScenePlay();
	void drawSceneStart();
	void drawScenePause();
	void drawSceneInstruction();
	void onMouseOverButtonStart();
	void onMouseOutButtonStart();
	void onMouseOverButtonInstruction();
	void onMouseOutButtonInstruction();

    void updateBoard(Board &board);
	void updateHoldingTile(Tile &tile);
	void updatePreparingTile(vector<Tile> &tiles);
	void updateScore(int score);

	shared_ptr<SDL_Window> getWindow();
};