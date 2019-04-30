#pragma once
#include <map>
#include <mutex>
#include "GameView.h"
#include "GameController.h"
#include "GameSound.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SceneStart.h"

using namespace std;

class Game {
typedef void (Game::*FunctionPointer)();
private:
	static const int FRAME_PER_SECOND;
	static const int SDL_DELAY_PER_FRAME;
	static shared_ptr<Game> instance;
	static shared_ptr<GameView> view;
	static shared_ptr<GameController> controller;
	static shared_ptr<GameSound> sound;

	static vector<pair<int, int>> tilePositions;
	static mutex eventMutex;
	static int lastTimeAccess;

	static bool newGame;
	static vector<SceneX> scenes;
	static SDL_TimerID autoSingleDropEvent;

	map<pair<int, int>, FunctionPointer> eventMap;
	SDL_Event event;
	const Uint8 *keystate;
	bool running;
	SDL_Point windowPosition;
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void start();
	void init();
	static void initStart();
	void initGamePlay();
	void initEventMap();
	void gameLoop();
	void gameLoopPause();
	void gameLoopPlay();
	void handleGamePause();
	void handleGameInstruction();
	void backToPreviousScene();
	void finish();
	static Uint32 autoSingleDrop(Uint32 interval, void *param);

	const Uint8* getKeystate();

	static void singleDropAndRender();
	void handleEventPlay();
};