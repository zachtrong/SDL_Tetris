#include "Game.h"

using namespace std;

const int Game::TILE_DROP_DELAY = 500;
const int Game::FRAME_PER_SECOND = 24;
const int Game::SDL_DELAY_PER_FRAME = 1000 / FRAME_PER_SECOND;
shared_ptr<Game> Game::instance(nullptr);
shared_ptr<GameView> Game::view(GameView::getInstance());
shared_ptr<GameController> Game::controller(GameController::getInstance());

vector<pair<int, int>> Game::tilePositions;

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
	SDL_TimerID autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);

	SDL_Event event;
	bool running = true;
	controller->genCurrentTile();
	view->updateBoard(*controller->getBoard());
	renderUpdatedPositions();
	while (running) {
		int frameStart = SDL_GetTicks();

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

Uint32 Game::autoSingleDrop(Uint32 interval, __attribute__((unused)) void *param) {
	if (controller->canDrop()) {
		controller->singleDrop();
		renderDeletedPositions();
		renderUpdatedPositions();
	} else {
		controller->genCurrentTile();
		renderUpdatedPositions();
	}
	return interval;
}

void Game::renderUpdatedPositions() {
	tilePositions = controller->getCurrentTilePositions();
	view->updateBoardChangedPositions(
		*controller->getBoard(), 
		tilePositions
	);
}

void Game::renderDeletedPositions() {
	view->updateBoardChangedPositions(
		*controller->getBoard(), 
		tilePositions
	);
}
