#include "Game.h"

using namespace std;

shared_ptr<Game> Game::instance(nullptr);

Game::Game() {

}

Game::~Game() {
}

shared_ptr<Game> Game::getInstance() {
	if (instance == nullptr) {
		instance = make_shared<Game>();
	}
	return instance;
}

void Game::start() {
	GameView::getInstance()->startSDL();
	processEvent();
}


void Game::processEvent() {
	SDL_Event event;
	bool running = true;
	while (running) {
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			running = false;
	}
}
