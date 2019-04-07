#include <iostream>
#include <SDL.h>
#include <memory>
#include <exception>
#pragma once

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game {
private:
	static shared_ptr<Game> instance;
	shared_ptr<SDL_Window> window;
	shared_ptr<SDL_Surface> screenSurface;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void start();
	void drawBackground();
	void init();
	void initWindow();
	void initScreenSurface();
};