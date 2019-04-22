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
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);

	SDL_Event event;
	bool running = true;
	controller->genCurrentTile();
	view->updateBoard(*controller->getBoard());
	while (running) {
		int frameStart = SDL_GetTicks();

		if(!SDL_PollEvent(&event)) {
			SDL_Delay(SDL_DELAY_PER_FRAME);
			continue;
		}

		switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYUP:
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0) {
					handleButtonSpace();
				} else if (event.key.keysym.sym == SDLK_LEFT) {
					if (event.key.repeat == 0) {
						handleButtonArrowLeft();
					} else {
						handleButtonArrowLeftContinuous();
					}
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					if (event.key.repeat == 0) {
						handleButtonArrowRight();
					} else {
						handleButtonArrowRightContinuous();
					}
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					if (event.key.repeat == 0) {
						handleButtonArrowDown();
					} else {
						handleButtonArrowDownContinuous();
					}
				} else if (event.key.keysym.sym == SDLK_UP) {
					if (event.key.repeat == 0) {
						handleButtonArrowUp();
					}
				}
				break;
			default:
				break;
		}

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < SDL_DELAY_PER_FRAME) {
			SDL_Delay(SDL_DELAY_PER_FRAME - frameTime);
		}
	}

	SDL_RemoveTimer(autoSingleDropEvent);
}

Uint32 Game::autoSingleDrop(Uint32 interval, __attribute__((unused)) void *param) {
	if (controller->canDrop()) {
		controller->singleDrop();
	} else {
		controller->genCurrentTile();
	}
	view->updateBoard(*controller->getBoard());
	return interval;
}

void Game::handleButtonArrowDown() {
	autoSingleDrop(0, nullptr);
	SDL_RemoveTimer(autoSingleDropEvent);
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
}

void Game::handleButtonArrowDownContinuous() {
	
}

void Game::handleButtonArrowUp() {
	controller->rotateRight();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowUpContinuous() {

}

void Game::handleButtonArrowLeft() {
	controller->moveLeft();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowLeftContinuous() {
	
}

void Game::handleButtonArrowRight() {
	controller->moveRight();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowRightContinuous() {
	
}

void Game::handleButtonSpace() {
	controller->hardDrop();
	view->updateBoard(*controller->getBoard());

	autoSingleDrop(0, nullptr);
	SDL_RemoveTimer(autoSingleDropEvent);
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
}
