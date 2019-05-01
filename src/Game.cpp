#include "Game.h"

using namespace std;

const int Game::FRAME_PER_SECOND = 24;
const int Game::SDL_DELAY_PER_FRAME = 1000 / FRAME_PER_SECOND;
shared_ptr<Game> Game::instance(nullptr);
shared_ptr<GameView> Game::view = GameView::getInstance();
shared_ptr<GameSound> Game::sound = GameSound::getInstance();

Game::Game()
	:scenes(),
	event(),
	keystate(),
	running(false)
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
	gameLoop();
}

void Game::init() {
	view->startSDL();
	sound->initSound();
	scenes.push_back(Scene::createSceneFromSceneType(START));
	scenes.back()->start();
	running = true;
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

		gameLoopActual();

		int frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < SDL_DELAY_PER_FRAME) {
			SDL_Delay(SDL_DELAY_PER_FRAME - frameTime);
		}
	}
}

void Game::gameLoopActual() {
	SceneType nextSceneType = scenes.back()->gameLoop(event);
	if (nextSceneType != scenes.back()->getType()) {
		if (nextSceneType == BACK_TO_PREVIOUS) {
			backToPreviousScene();
		} else {
			changeToNextScene(nextSceneType);
		}
	}
}

void Game::backToPreviousScene() {
	scenes.pop_back();
	scenes.back()->start();
}

void Game::changeToNextScene(SceneType nextSceneType) {
	scenes.push_back(Scene::createSceneFromSceneType(nextSceneType));
	scenes.back()->start();
}

const Uint8* Game::getKeystate() {
	return keystate;
}