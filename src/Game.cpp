#include "Game.h"

using namespace std;

const int Game::TILE_DROP_DELAY = 750;
const int Game::FRAME_PER_SECOND = 24;
const int Game::SDL_DELAY_PER_FRAME = 1000 / FRAME_PER_SECOND;
const int Game::DELAY_CONTINUOUS_KEY = 200;
shared_ptr<Game> Game::instance(nullptr);
shared_ptr<GameView> Game::view(GameView::getInstance());
shared_ptr<GameController> Game::controller(GameController::getInstance());
shared_ptr<GameSound> Game::sound(GameSound::getInstance());

vector<pair<int, int>> Game::tilePositions;
mutex Game::eventMutex;
vector<Scene> Game::scenes;
bool Game::newGame = false;
SDL_TimerID Game::autoSingleDropEvent;

Game::Game() 
	:eventMap(),
	event(),
	keystate(),
	running(false),
	mouseOverStart(false),
	mouseOverInstruction(false)
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
	scenes.push_back(PLAY);
	view->drawScenePlay();
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
	if (newGame) {
		newGame = false;
		controller->genCurrentTile();
	}
	view->updateBoard(*controller->getBoard());
	view->updateHoldingTile(*controller->getHoldingTile());
	view->updatePreparingTile(*controller->getPreparingTiles());
	view->updateScore(controller->getScore());
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
	eventMap.emplace(make_pair(SDLK_z, 0), &Game::handleButtonZ);
	eventMap.emplace(make_pair(SDLK_x, 0), &Game::handleButtonX);
	eventMap.emplace(make_pair(SDLK_c, 0), &Game::handleButtonC);
	eventMap.emplace(make_pair(SDLK_LSHIFT, 0), &Game::handleButtonShift);
	eventMap.emplace(make_pair(SDLK_ESCAPE, 0), &Game::handleButtonEscape);
	eventMap.emplace(make_pair(SDLK_p, 0), &Game::handleButtonP);
	eventMap.emplace(make_pair(SDLK_LCTRL, 0), &Game::handleButtonCtrl);
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

void Game::gameLoopStart() {
	if (event.type == SDL_MOUSEMOTION) {
		handleMouseOver();
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		handleMouseClick();
	}
}

bool Game::isMouseOverStartButton() {
	return isMouseOverRect(Constants::RECT_START_BUTTON);
}

bool Game::isMouseOverRect(const SDL_Rect &rect) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return (
		rect.x <= mouseX
		&& mouseX <= rect.x + rect.w
		&& rect.y <= mouseY
		&& mouseY <= rect.y + rect.h
	);
}

bool Game::isMouseOverInstructionButton() {
	return isMouseOverRect(Constants::RECT_INSTRUCTION_BUTTON);
}

void Game::handleMouseClick() {
	if (isMouseOverStartButton()) {
		initGamePlay();
	} else if (isMouseOverInstructionButton()) {
		handleGameInstruction();
	}
}

void Game::handleMouseOver() {
	if (isMouseOverStartButton()) {
		handleMouseOverStart();
	} else if (isMouseOverInstructionButton()) {
		handleMouseOverInstruction();
	} else {
		handleMouseOverBackground();
	}
}

void Game::handleMouseOverStart() {
	if (!mouseOverStart) {
		mouseOverStart = true;
		view->onMouseOverButtonStart();
	}
	if (mouseOverInstruction) {
		mouseOverInstruction = false;
		view->onMouseOutButtonInstruction();
	}
}

void Game::handleMouseOverInstruction() {
	if (!mouseOverInstruction) {
		mouseOverInstruction = true;
		view->onMouseOverButtonInstruction();
	}
	if (mouseOverStart) {
		mouseOverStart = false;
		view->onMouseOutButtonStart();
	}
}

void Game::handleMouseOverBackground() {
	if (mouseOverStart) {
		mouseOverStart = false;
		view->onMouseOutButtonStart();
	}
	if (mouseOverInstruction) {
		mouseOverInstruction = false;
		view->onMouseOutButtonInstruction();
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

void Game::gameLoopPlay() {
	lock_guard<mutex> lock(eventMutex);
	handleEventPlay();
}

void Game::handleEventPlay() {
	if (event.type == SDL_KEYDOWN) {
		pair<int, int> eventKey = make_pair(event.key.keysym.sym, event.key.repeat);
		auto it = eventMap.find(eventKey);
		if (it != eventMap.end()) {
			FunctionPointer fp = it->second;
			(this->*fp)();
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
		if (!controller->genCurrentTile()) {
			SDL_RemoveTimer(autoSingleDropEvent);
			initStart();
			return;
		}
		view->updatePreparingTile(*controller->getPreparingTiles());
	}
	view->updateBoard(*controller->getBoard());
	view->updateScore(controller->getScore());
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
	if (!controller->genCurrentTile()) {
		initStart();
		return;
	}
	view->updateBoard(*controller->getBoard());
	view->updateScore(controller->getScore());
	view->updatePreparingTile(*controller->getPreparingTiles());
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
}

void Game::handleButtonZ() {
	controller->rotateLeft();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonX() {
	controller->rotateRight();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonC() {
	controller->hold();
	view->updateBoard(*controller->getBoard());
	view->updateHoldingTile(*controller->getHoldingTile());
	view->updatePreparingTile(*controller->getPreparingTiles());
}

void Game::handleButtonCtrl() {
	controller->rotateLeft();
	view->updateBoard(*controller->getBoard());
}

void Game::handleButtonShift() {
	controller->hold();
	view->updateBoard(*controller->getBoard());
	view->updateHoldingTile(*controller->getHoldingTile());
	view->updatePreparingTile(*controller->getPreparingTiles());
}

void Game::handleButtonEscape() {
	handleGamePause();
}

void Game::handleButtonP() {
	handleGamePause();
}

void Game::finish() {
	SDL_RemoveTimer(autoSingleDropEvent);
}
