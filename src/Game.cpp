#include "Game.h"

using namespace std;

const int Game::TILE_DROP_DELAY = 500;
const int Game::FRAME_PER_SECOND = 24;
const int Game::SDL_DELAY_PER_FRAME = 1000 / FRAME_PER_SECOND;
const int Game::DELAY_CONTINUOUS_KEY = 200;
shared_ptr<Game> Game::instance(nullptr);
shared_ptr<GameView> Game::view(GameView::getInstance());
shared_ptr<GameController> Game::controller(GameController::getInstance());
shared_ptr<GameSound> Game::sound(GameSound::getInstance());

vector<pair<int, int>> Game::tilePositions;
mutex Game::eventMutex;

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
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw Exception(SDL_GetError());
	}
	view->startSDL();
	sound->initSound();
	init();
	gameLoop();
	finish();
}

void Game::init() {
	initEventMap();

	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
	running = true;
	controller->genCurrentTile();
	view->updateBoard(*controller->getBoard());
}

void Game::initEventMap() {
	eventMap.emplace(make_pair(SDLK_SPACE, 0), &Game::handleButtonSpace);
	eventMap.emplace(make_pair(SDLK_LEFT, 0), &Game::handleButtonArrowLeft);
	eventMap.emplace(make_pair(SDLK_LEFT, 1), &Game::handleButtonArrowLeftContinuous);
	eventMap.emplace(make_pair(SDLK_RIGHT, 0), &Game::handleButtonArrowRight);
	eventMap.emplace(make_pair(SDLK_RIGHT, 1), &Game::handleButtonArrowRightContinuous);
	eventMap.emplace(make_pair(SDLK_DOWN, 0), &Game::handleButtonArrowDown);
	eventMap.emplace(make_pair(SDLK_DOWN, 1), &Game::handleButtonArrowDownContinuous);
	eventMap.emplace(make_pair(SDLK_UP, 0), &Game::handleButtonArrowUp);
}

void Game::gameLoop() {
	keystate = SDL_GetKeyboardState(NULL);
	while (running) {
		int frameStart = SDL_GetTicks();

		if(!SDL_PollEvent(&event)) {
			SDL_Delay(SDL_DELAY_PER_FRAME);
			continue;
		}
		SDL_PumpEvents();

		handleEvent();

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < SDL_DELAY_PER_FRAME) {
			SDL_Delay(SDL_DELAY_PER_FRAME - frameTime);
		}
	}
}

void Game::handleEvent() {
	lock_guard<mutex> lock(eventMutex);
	if (event.type == SDL_QUIT) {
		running = false;
		return;
	}
	if (event.type == SDL_KEYDOWN) {
		pair<int, int> eventKey = make_pair(event.key.keysym.sym, event.key.repeat);
		auto it = eventMap.find(eventKey);
		if (it != eventMap.end()) {
			FunctionPointer fp = it->second;
			(this->*fp)();

			view->updateScore(controller->getScore());
		}
	}
}

Uint32 Game::autoSingleDrop(Uint32 interval, __attribute__((unused)) void *param) {
	lock_guard<mutex> lock(eventMutex);
	singleDropAndRender();
	return interval;
}

void Game::singleDropAndRender() {
	if (controller->canDrop()) {
		controller->singleDrop();
	} else {
		controller->collapse();
		controller->genCurrentTile();
	}
	view->updateBoard(*controller->getBoard());
	view->updateScore(controller->getScore());
}

void Game::handleButtonArrowDown() {
	controller->singleDrop();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowDownContinuous() {
	SDL_RemoveTimer(autoSingleDropEvent);
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		controller->singleDrop();
		view->updateBoard(*controller->getBoard());
		currentTick = lastTimeAccess;
	}
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
}

void Game::handleButtonArrowUp() {
	controller->rotateRight();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowUpContinuous() {
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		controller->rotateRight();
		view->updateBoard(*controller->getBoard());
		currentTick = lastTimeAccess;
	}
}

void Game::handleButtonArrowLeft() {
	controller->moveLeft();
	if (keystate[SDL_SCANCODE_DOWN]) {
		controller->singleDrop();
	}
	sound->playMoveLeftRight();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowLeftContinuous() {
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		handleButtonArrowLeft();
		currentTick = lastTimeAccess;
	}
}

void Game::handleButtonArrowRight() {
	controller->moveRight();
	if (keystate[SDL_SCANCODE_DOWN]) {
		controller->singleDrop();
	}
	sound->playMoveLeftRight();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonArrowRightContinuous() {
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		handleButtonArrowRight();
		currentTick = lastTimeAccess;
	}
}

void Game::handleButtonSpace() {
	SDL_RemoveTimer(autoSingleDropEvent);
	controller->hardDrop();
	if (controller->collapse()) {
		sound->playLineClear();
	} else {
		sound->playHardDrop();
	}
	controller->genCurrentTile();
	view->updateBoard(*controller->getBoard());
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
}


void Game::finish() {
	SDL_RemoveTimer(autoSingleDropEvent);
}