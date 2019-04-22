#pragma once
#include <map>
#include "GameView.h"
#include "GameController.h"
#include "SDL.h"

using namespace std;

class Game {
typedef void (Game::*FunctionPointer)();
private:
	static const int TILE_DROP_DELAY;
	static const int FRAME_PER_SECOND;
	static const int SDL_DELAY_PER_FRAME;
	static shared_ptr<Game> instance;
	static shared_ptr<GameView> view;
	static shared_ptr<GameController> controller;

	static vector<pair<int, int>> tilePositions;
	map<pair<int, int>, FunctionPointer> eventMap;
	SDL_TimerID autoSingleDropEvent;
	SDL_Event event;
	bool running;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void start();
	void init();
	void initEventMap();
	void gameLoop();
	void finish();
	static Uint32 autoSingleDrop(Uint32 interval, void *param);

	void handleEvent();
	void handleButtonArrowDown();
	void handleButtonArrowDownContinuous();
	void handleButtonArrowUp();
	void handleButtonArrowUpContinuous();
	void handleButtonArrowLeft();
	void handleButtonArrowLeftContinuous();
	void handleButtonArrowRight();
	void handleButtonArrowRightContinuous();
	void handleButtonSpace();
};