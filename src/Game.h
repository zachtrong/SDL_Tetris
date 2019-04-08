#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <memory>
#include <exception>
#include "Exception.h"
#include "Constants.h"
#include "PointerDefinition.h"
#pragma once

using namespace std;

class Game {
private:
	static const int SCREEN_WIDTH;
	static const int SCREEN_HEIGHT;

	static shared_ptr<Game> instance;
	shared_ptr<SDL_Window> window;
	shared_ptr<SDL_Renderer> renderer;
	shared_ptr<SDL_Texture> texture;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void start();
	void drawBackground();
	void processEvent();
	void init();
	void initWindow();
	void initRenderer();
	void initImage();
	void initTexture();
	SDL_Texture* loadBackgroundTexture();
};