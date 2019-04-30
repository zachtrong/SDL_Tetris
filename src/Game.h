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
private:
	static const int FRAME_PER_SECOND;
	static const int SDL_DELAY_PER_FRAME;
	static shared_ptr<Game> instance;

	SDL_Event event;
	const Uint8 *keystate;
	bool running;
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