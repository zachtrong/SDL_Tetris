#include "Game.h"
#include "Constants.h"
#include "PointerDefinition.h"

using namespace std;

shared_ptr<Game> Game::instance(nullptr);

Game::Game(): 
	window(nullptr),
	screenSurface(nullptr) {

}

Game::~Game() {
	SDL_Quit();
}

shared_ptr<Game> Game::getInstance() {
	if (instance == nullptr) {
		instance = make_shared<Game>();
	}
	return instance;
}

void Game::start() {
	try {
		init();
	} catch (...) {
		printf("Something went wrong!");
		return;
	}
	
	drawBackground();

	printf("initialization successful!\n");
	system("pause");
}

void Game::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw SDL_GetError();
		return;
	}
	initWindow();
	initScreenSurface();
}

void Game::initWindow() {
	window = PointerDefinition::createSdlWindowPointer(
		SDL_CreateWindow(
			Constants::GAME_TITLE, 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, 
			SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN
		)
	);
	if (window == nullptr) {
		throw SDL_GetError();
		return;
	}
}

void Game::initScreenSurface() {
	screenSurface = PointerDefinition::createSdlSurfacePointer(
		SDL_GetWindowSurface(window.get())
	);
}

void Game::drawBackground() {
	SDL_FillRect(screenSurface.get(), NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
	SDL_UpdateWindowSurface(window.get());
	SDL_Delay(2000);
}