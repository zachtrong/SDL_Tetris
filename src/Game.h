#pragma once
#include "GameView.h"
#include "SDL.h"

using namespace std;

class Game {
private:
	static shared_ptr<Game> instance;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void processEvent();
	void start();
};