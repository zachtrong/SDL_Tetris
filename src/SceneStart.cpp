#include "SceneStart.h"

using namespace std;

const SDL_Rect SceneStart::RECT_BUTTON_START = {
	390, 271,
	143, 70
};
const SDL_Rect SceneStart::RECT_BUTTON_INSTRUCTION = {
	402, 366,
	120, 33
};
const SDL_Rect SceneStart::RECT_BUTTON_HIGH_SCORE = {
	403, 424,
	120, 34
};

SceneStart::SceneStart()
	:buttonStart(new DisplayObject(
        "assets/textures/button_start.png", 
        RECT_BUTTON_START
    )),
    buttonInstruction(new DisplayObject(
        "assets/textures/button_instruction.png", 
        RECT_BUTTON_INSTRUCTION
    )),
	buttonHighScore(new DisplayObject(
		"assets/textures/button_high_score.png",
		RECT_BUTTON_HIGH_SCORE
	))
{
    background = make_shared<DisplayObject>("assets/textures/scene_start.png", RECT_BACKGROUND);
}

SceneStart::~SceneStart() {

}

void SceneStart::start() {
	sceneType = nextSceneType = START;
    view->renderDisplayObject(background);

	clearButton();
	setButtonDefault(background, nullptr);
	function<void ()> buttonStartCallback = bind(
		&SceneStart::onClickButtonStart,
		this
	);
	addButton(buttonStart, buttonStartCallback);
	function<void ()> buttonInstructionCallback = bind(
		&SceneStart::onClickButtonInstruction,
		this
	);
	addButton(buttonInstruction, buttonInstructionCallback);
	function<void ()> buttonHighScoreCallback = bind(
		&SceneStart::onClickButtonHighScore,
		this
	);
	addButton(buttonHighScore, buttonHighScoreCallback);
}

SceneType SceneStart::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
	return nextSceneType;
}

void SceneStart::onClickButtonStart() {
	nextSceneType = PLAY;
}

void SceneStart::onClickButtonInstruction() {
	nextSceneType = INSTRUCTION;
}

void SceneStart::onClickButtonHighScore() {
	nextSceneType = ENDGAME;
}