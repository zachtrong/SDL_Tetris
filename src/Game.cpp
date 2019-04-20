#include "Game.h"

using namespace std;

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


void Game::processEvent() {
	SDL_Event event;
	bool running = true;
	while (running) {
		controller->genCurrentTile();
		controller->addCurrentTileToBoard();
		view->updateBoard(*controller->getBoard());
		controller->deleteCurrentTileFromBoard();

		if(!SDL_PollEvent(&event)) continue;
		if (event.type == SDL_QUIT)
			running = false;
	}
}
