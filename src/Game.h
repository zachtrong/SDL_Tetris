#pragma once
#include "GameView.h"
#include "GameController.h"
#include "SDL.h"

using namespace std;

class Game {
private:
	static const int TILE_DROP_DELAY;
	static const int FRAME_PER_SECOND;
	static const int SDL_DELAY_PER_FRAME;
	static shared_ptr<Game> instance;
	static shared_ptr<GameView> view;
	static shared_ptr<GameController> controller;

	static vector<pair<int, int>> tilePositions;
	SDL_TimerID autoSingleDropEvent;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void processEvent();
	void start();
	static Uint32 autoSingleDrop(Uint32 interval, void *param);

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