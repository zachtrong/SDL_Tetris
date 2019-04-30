#include "Game.h"

using namespace std;

const int Game::FRAME_PER_SECOND = 24;
const int Game::SDL_DELAY_PER_FRAME = 1000 / FRAME_PER_SECOND;
shared_ptr<Game> Game::instance(nullptr);
shared_ptr<GameView> Game::view(GameView::getInstance());
shared_ptr<GameController> Game::controller(GameController::getInstance());
shared_ptr<GameSound> Game::sound(GameSound::getInstance());

vector<pair<int, int>> Game::tilePositions;
mutex Game::eventMutex;
vector<SceneX> Game::scenes;
bool Game::newGame = false;
SDL_TimerID Game::autoSingleDropEvent;

Game::Game() 
	:eventMap(),
	event(),
	keystate(),
	running(false),
{
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
	init();
	initStart();
	gameLoop();
	finish();
}

void Game::init() {
	view->startSDL();
	sound->initSound();
	shared_ptr<SDL_Window> window = view->getWindow();
	SDL_GetWindowPosition(window.get(), &windowPosition.x, &windowPosition.y);
	initEventMap();
	running = true;
}

void Game::initStart() {
	scenes.push_back(START);
	view->drawSceneStart();
	controller->clearAll();
	newGame = true;
}

void Game::initGamePlay() {
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

		if (event.type == SDL_QUIT) {
			running = false;
			return;
		}

		switch (scenes.back()) {
			case START:
				gameLoopStart();
				break;
			case PAUSE:
			case INSTRUCTION:
				gameLoopPause();
				break;
			case PLAY:
				gameLoopPlay();
				break;
		}

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < SDL_DELAY_PER_FRAME) {
			SDL_Delay(SDL_DELAY_PER_FRAME - frameTime);
		}
	}
}

void Game::gameLoopPause() {
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_ESCAPE
			|| event.key.keysym.sym == SDLK_p) {
			if (event.key.repeat == 0) {
				handleGamePause();
			}
		}
	} 
}

void Game::handleGamePause() {
	if (scenes.back() == PAUSE) {
		backToPreviousScene();
	} else {
		SDL_RemoveTimer(autoSingleDropEvent);
		scenes.push_back(PAUSE);
		view->drawScenePause();
	}
}

void Game::handleGameInstruction() {
	if (scenes.back() == INSTRUCTION) {
		backToPreviousScene();
	} else {
		SDL_RemoveTimer(autoSingleDropEvent);
		scenes.push_back(INSTRUCTION);
		view->drawSceneInstruction();
	}
}

void Game::backToPreviousScene() {
	scenes.pop_back();
	if (scenes.back() == START) {
		initStart();
	} else if (scenes.back() == PLAY) {
		initGamePlay();
	}
}

const Uint8* Game::getKeystate() {
	return keystate;
}

void Game::finish() {
	SDL_RemoveTimer(autoSingleDropEvent);
}
