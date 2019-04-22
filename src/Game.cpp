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
					controller->hardDrop();

					view->updateBoard(*controller->getBoard());

					autoSingleDrop(0, nullptr);
					SDL_RemoveTimer(autoSingleDropEvent);
					autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
				} else if (event.key.keysym.sym == SDLK_LEFT) {
					static int lastTimePress = SDL_GetTicks();
					if (event.key.repeat == 0) {
						controller->moveLeft();
						view->updateBoard(*controller->getBoard());
					} else {
						int currentTick = SDL_GetTicks();
						if (currentTick - lastTimePress > 200) {
							lastTimePress = currentTick;
							controller->moveLeft();
							view->updateBoard(*controller->getBoard());
						}
					}
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					static int lastTimePress = SDL_GetTicks();
					if (event.key.repeat == 0) {
						controller->moveRight();
						view->updateBoard(*controller->getBoard());
					} else {
						int currentTick = SDL_GetTicks();
						if (currentTick - lastTimePress > 200) {
							lastTimePress = currentTick;
							controller->moveRight();
							view->updateBoard(*controller->getBoard());
						}
					}
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					static int lastTimePress = SDL_GetTicks();
					if (event.key.repeat == 0) {
						autoSingleDrop(0, nullptr);
						SDL_RemoveTimer(autoSingleDropEvent);
						autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
					} else {
						int currentTick = SDL_GetTicks();
						if (currentTick - lastTimePress > 200) {
							autoSingleDrop(0, nullptr);
							SDL_RemoveTimer(autoSingleDropEvent);
							autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
						}
					}
				} else if (event.key.keysym.sym == SDLK_UP) {
					static int lastTimePress = SDL_GetTicks();
					if (event.key.repeat == 0) {
						controller->rotateRight();
						view->updateBoard(*controller->getBoard());
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