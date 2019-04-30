#pragma once
#include <map>
#include <mutex>
#include "GameView.h"
#include "GameController.h"
#include "GameSound.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SceneStart.h"
#include "ScenePlay.h"
#include "ScenePause.h"
#include "SceneInstruction.h"

using namespace std;

class Game {
private:
	static const int FRAME_PER_SECOND;
	static const int SDL_DELAY_PER_FRAME;
	static shared_ptr<Game> instance;
	static shared_ptr<GameView> view;
	static shared_ptr<GameSound> sound;

	vector<shared_ptr<Scene>> scenes;
	SDL_Event event;
	const Uint8 *keystate;
	bool running;

	void init();
	void gameLoop();
public:
	static shared_ptr<Game> getInstance();

	Game();
	virtual ~Game();

	void start();
	const Uint8* getKeystate();
};