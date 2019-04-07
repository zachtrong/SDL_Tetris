#include "Game.h"
#include "Constants.h"
#include "PointerDefinition.h"

using namespace std;

Game::Game() {

}

Game::~Game() {
	SDL_Quit();
}

void Game::start() {
	shared_ptr<SDL_Window> window(nullptr);
	shared_ptr<SDL_Surface> screenSurface(nullptr);

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return;
	}
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
	if (window.get() == NULL) {
		printf("Window could not create: %s\n", SDL_GetError());
		return;
	}
	screenSurface = PointerDefinition::createSdlSurfacePointer(
		SDL_GetWindowSurface(window.get())
	);
	SDL_FillRect(screenSurface.get(), NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));
	SDL_UpdateWindowSurface(window.get());
	SDL_Delay(2000);

	printf("initialization successful!\n");
	system("pause");
}