#include "Game.h"

using namespace std;

const int Game::TILE_DROP_DELAY = 500;
const int Game::FRAME_PER_SECOND = 24;
const int Game::SDL_DELAY_PER_FRAME = 1000 / FRAME_PER_SECOND;
shared_ptr<Game> Game::instance(nullptr);
shared_ptr<GameView> Game::view(GameView::getInstance());
shared_ptr<GameController> Game::controller(GameController::getInstance());

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

Uint32 Game::autoSingleDrop(Uint32 interval, void *param) {
	if (controller->canDrop()) {
		controller->singleDrop();
	} else {
		controller->genCurrentTile();
	}
	return interval;
}

void Game::processEvent() {
	SDL_TimerID autoSingleDropEvent = SDL_AddTimer(1000, autoSingleDrop, nullptr);

	SDL_Event event;
	bool running = true;
	controller->genCurrentTile();
	while (running) {
		int frameStart = SDL_GetTicks();
		view->updateBoard(*controller->getBoard());

		if(!SDL_PollEvent(&event)) continue;
		if (event.type == SDL_QUIT)
			running = false;

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < FRAME_PER_SECOND) {
			SDL_Delay(FRAME_PER_SECOND - frameTime);
		}
	}

	SDL_RemoveTimer(autoSingleDropEvent);
}
