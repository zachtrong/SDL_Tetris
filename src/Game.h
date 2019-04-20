#pragma once
#include "GameView.h"
#include "GameController.h"
#include "SDL.h"

using namespace std;

class Game {
private:
	static shared_ptr<Game> instance;
	static shared_ptr<GameView> view;
	static shared_ptr<GameController> controller;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void processEvent();
	void start();
};