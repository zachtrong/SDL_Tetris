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

Game::Game() 
	:eventMap(),
	autoSingleDropEvent(),
	event(),
	keystate(),
	running(false),
	scene(START),
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
	scene = START;
}

void Game::initGamePlay() {
	scene = PLAY;
	view->drawScenePlay();
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, nullptr);
	controller->genCurrentTile();
	view->updateBoard(*controller->getBoard());
	view->updatePreparingTile(*controller->getPreparingTiles());
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

		if (scene == START) {
			gameLoopStart();
		} else if (scene == PAUSE) {
			gameLoopPause();
		} else if (scene == PLAY) {
			gameLoopPlay();
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
		//TODO
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
	//TODO
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
		view->updatePreparingTile(*controller->getPreparingTiles());
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
	//TODO
}

void Game::handleButtonP() {
	//TODO
}

void Game::finish() {
	SDL_RemoveTimer(autoSingleDropEvent);
}