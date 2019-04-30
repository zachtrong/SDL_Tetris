#include "ScenePlay.h"
#include "Game.h"

using namespace std;

const SDL_Rect ScenePlay::RECT_BORDER_LEFT = {
	Constants::SCREEN_OFFSET - Constants::BOARD_BORDER_SIZE, 0,
	Constants::BOARD_BORDER_SIZE, Constants::SCREEN_HEIGHT
};
const SDL_Rect ScenePlay::RECT_BORDER_RIGHT = {
	Constants::SCREEN_OFFSET*2, 0,
	Constants::BOARD_BORDER_SIZE, Constants::SCREEN_HEIGHT
};
const SDL_Rect ScenePlay::RECT_BACKGROUND_SCORING = {
	0, 353, 
	Constants::SCREEN_OFFSET - Constants::BOARD_BORDER_SIZE, 54
};
const SDL_Rect ScenePlay::RECT_HOLDING = {89, 100, 122, 105};
const SDL_Rect ScenePlay::RECT_PREPARING[4] = {
	{Constants::SCREEN_OFFSET*2 + 89, 100, 122, 105},
	{Constants::SCREEN_OFFSET*2 + 98, 234, 105, 90},
	{Constants::SCREEN_OFFSET*2 + 105, 353, 91, 78},
	{Constants::SCREEN_OFFSET*2 + 105, 460, 91, 78}
};

const SDL_Color ScenePlay::BORDER_COLOR = {0x71, 0x71, 0x71, 0xff};
const SDL_Color ScenePlay::BACKGROUND_COLOR = {0x55, 0x60, 0x60, 0xff};

const int ScenePlay::TILE_DROP_DELAY = 750;
const int ScenePlay::DELAY_CONTINUOUS_KEY = 200;

mutex ScenePlay::eventMutex;

ScenePlay::ScenePlay()
	:newGame(true),
	autoSingleDropEvent(),
	eventMap({
		{{SDLK_SPACE, 0}, &ScenePlay::handleButtonSpace},
		{{SDLK_LEFT, 0}, &ScenePlay::handleButtonArrowLeft},
		{{SDLK_LEFT, 1}, &ScenePlay::handleButtonArrowLeftContinuous},
		{{SDLK_RIGHT, 0}, &ScenePlay::handleButtonArrowRight},
		{{SDLK_RIGHT, 1}, &ScenePlay::handleButtonArrowRightContinuous},
		{{SDLK_DOWN, 0}, &ScenePlay::handleButtonArrowDown},
		{{SDLK_DOWN, 1}, &ScenePlay::handleButtonArrowDownContinuous},
		{{SDLK_UP, 0}, &ScenePlay::handleButtonArrowUp},
		{{SDLK_z, 0}, &ScenePlay::handleButtonZ},
		{{SDLK_x, 0}, &ScenePlay::handleButtonX},
		{{SDLK_c, 0}, &ScenePlay::handleButtonC},
		{{SDLK_LSHIFT, 0}, &ScenePlay::handleButtonShift},
		{{SDLK_ESCAPE, 0}, &ScenePlay::handleButtonEscape},
		{{SDLK_p, 0}, &ScenePlay::handleButtonP},
		{{SDLK_LCTRL, 0}, &ScenePlay::handleButtonCtrl}
	}),
	textHold(new FontObject("hold", 36, SDL_Rect{121, 41, 0, 0})),
	textNext(new FontObject("next", 36, SDL_Rect{731, 41, 0, 0})),
	textScore(new FontObject("score", 36, SDL_Rect{112, 298, 0, 0})),
	textScoring(new FontObject("1000", 50, SDL_Rect{144, 353, 0, 0})),
	textFooter(new FontObject("press ESC or P to pause", 24, SDL_Rect{59, 555, 0, 0})),
	borderLeft(new RectObject(RECT_BORDER_LEFT, BORDER_COLOR)),
	borderRight(new RectObject(RECT_BORDER_RIGHT, BORDER_COLOR)),
	backgroundRect(new RectObject(RECT_BACKGROUND, BACKGROUND_COLOR)),
	backgroundScoring(new RectObject(RECT_BACKGROUND_SCORING, BACKGROUND_COLOR))
{
	Board &board = *controller->getBoard();
    for (int i = Constants::BOARD_HEIGHT/2; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
			shared_ptr<TileObject> tileObject = make_shared<TileObject>(
				board[i][j].getType(), 
				board[i][j].getPositionOnWindow()
			);
			boardTileObjects.push_back(tileObject);
        }
    }
}

ScenePlay::~ScenePlay() {
	SDL_RemoveTimer(autoSingleDropEvent);
}

void ScenePlay::start() {
	sceneType = nextSceneType = PLAY;
	view->renderRectObject({backgroundRect, borderLeft, borderRight});
	view->renderFontObject({textHold, textNext, textScore, textFooter});

	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, this);
	if (newGame) {
		newGame = false;
		controller->clearAll();
		controller->genCurrentTile();
	}
	updateViewBoard(*controller->getBoard());
	updateViewHoldingTile(*controller->getHoldingTile());
	updateViewPreparingTile(*controller->getPreparingTiles());
	updateViewScoring(controller->getScore());
}

SceneType ScenePlay::gameLoop(SDL_Event &event) {
	lock_guard<mutex> lock(eventMutex);
	if (nextSceneType != sceneType) {
		return nextSceneType;
	}
	if (event.type == SDL_KEYDOWN) {
		pair<int, int> eventKey = make_pair(event.key.keysym.sym, event.key.repeat);
		auto it = eventMap.find(eventKey);
		if (it != eventMap.end()) {
			FunctionPointer fp = it->second;
			(this->*fp)();
		}
	}
	return nextSceneType;
}

void ScenePlay::updateViewScoring(int scoring) {
	view->renderRectObject(backgroundScoring);
	textScoring->text = to_string(scoring);
	view->renderFontObject(textScoring, true);
}

void ScenePlay::updateViewBoard(Board &board) {
	int boardTileObjectId = 0;
    for (int i = Constants::BOARD_HEIGHT/2; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
			boardTileObjects[boardTileObjectId++]->type = board[i][j].getType();
        }
    }
	view->renderTileObject(boardTileObjects);
}

void ScenePlay::updateViewHoldingTile(Tile &tile) {
	view->renderFullTileObject(make_shared<FullTileObject>(tile.getType(), RECT_HOLDING));
}

void ScenePlay::updateViewPreparingTile(vector<Tile> preparingTiles) {
	if (preparingTiles.size() != 4) {
		throw Exception("Preparing tiles should have size 4!");
	}
	vector<shared_ptr<FullTileObject>> fullTileObjects;
	for (size_t i = 0; i < preparingTiles.size(); ++i) {
		fullTileObjects.push_back(make_shared<FullTileObject>(preparingTiles[i].getType(), RECT_PREPARING[i]));
	}
	view->renderFullTileObject(fullTileObjects);
}

Uint32 ScenePlay::autoSingleDrop(Uint32 interval, void *param) {
	lock_guard<mutex> lock(eventMutex);
	auto scenePlay = reinterpret_cast<ScenePlay*>(param);
	scenePlay->singleDropAndRender();
	return interval;
}

void ScenePlay::singleDropAndRender() {
	if (controller->canDrop()) {
		controller->singleDrop();
	} else {
		controller->collapse();
		if (!controller->genCurrentTile()) {
			SDL_RemoveTimer(autoSingleDropEvent);
			nextSceneType = BEFORE_ENDGAME;
			return;
		}
		updateViewPreparingTile(*controller->getPreparingTiles());
	}
	updateViewBoard(*controller->getBoard());
	updateViewScoring(controller->getScore());
}

void ScenePlay::handleButtonArrowDown() {
	controller->singleDrop();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonArrowDownContinuous() {
	SDL_RemoveTimer(autoSingleDropEvent);
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		controller->singleDrop();
		updateViewBoard(*controller->getBoard());
		currentTick = lastTimeAccess;
	}
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, this);
}

void ScenePlay::handleButtonArrowUp() {
	controller->rotateRight();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonArrowUpContinuous() {
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		controller->rotateRight();
		updateViewBoard(*controller->getBoard());
		currentTick = lastTimeAccess;
	}
}

void ScenePlay::handleButtonArrowLeft() {
	controller->moveLeft();
	if (Game::getInstance()->getKeystate()[SDL_SCANCODE_DOWN]) {
		controller->singleDrop();
	}
	sound->playMoveLeftRight();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonArrowLeftContinuous() {
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		handleButtonArrowLeft();
		currentTick = lastTimeAccess;
	}
}

void ScenePlay::handleButtonArrowRight() {
	controller->moveRight();
	if (Game::getInstance()->getKeystate()[SDL_SCANCODE_DOWN]) {
		controller->singleDrop();
	}
	sound->playMoveLeftRight();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonArrowRightContinuous() {
	static int lastTimeAccess = SDL_GetTicks();
	int currentTick = SDL_GetTicks();
	if (currentTick - lastTimeAccess > DELAY_CONTINUOUS_KEY) {
		handleButtonArrowRight();
		currentTick = lastTimeAccess;
	}
}

void ScenePlay::handleButtonSpace() {
	SDL_RemoveTimer(autoSingleDropEvent);
	controller->hardDrop();
	if (controller->collapse()) {
		sound->playLineClear();
	} else {
		sound->playHardDrop();
	}
	if (!controller->genCurrentTile()) {
		nextSceneType = BEFORE_ENDGAME;
		return;
	}
	updateViewBoard(*controller->getBoard());
	updateViewPreparingTile(*controller->getPreparingTiles());
	updateViewScoring(controller->getScore());
	autoSingleDropEvent = SDL_AddTimer(TILE_DROP_DELAY, autoSingleDrop, this);
}

void ScenePlay::handleButtonZ() {
	controller->rotateLeft();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonX() {
	controller->rotateRight();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonC() {
	controller->hold();
	updateViewBoard(*controller->getBoard());
	updateViewHoldingTile(*controller->getHoldingTile());
	updateViewPreparingTile(*controller->getPreparingTiles());
}

void ScenePlay::handleButtonCtrl() {
	controller->rotateLeft();
	updateViewBoard(*controller->getBoard());
}

void ScenePlay::handleButtonShift() {
	controller->hold();
	updateViewBoard(*controller->getBoard());
	updateViewHoldingTile(*controller->getHoldingTile());
	updateViewPreparingTile(*controller->getPreparingTiles());
}

void ScenePlay::handleButtonEscape() {
	SDL_RemoveTimer(autoSingleDropEvent);
	nextSceneType = PAUSE;
}

void ScenePlay::handleButtonP() {
	SDL_RemoveTimer(autoSingleDropEvent);
	nextSceneType = PAUSE;
}