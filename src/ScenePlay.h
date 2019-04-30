#pragma once
#include "Scene.h"

using namespace std;

class ScenePlay : public Scene {
typedef void (ScenePlay::*FunctionPointer)();
private:
	static const SDL_Rect RECT_BORDER_LEFT;
	static const SDL_Rect RECT_BORDER_RIGHT;
	static const SDL_Rect RECT_BACKGROUND_SCORING;
	static const SDL_Rect RECT_HOLDING;
	static const SDL_Rect RECT_PREPARING[4];
	static const SDL_Color BORDER_COLOR;
	static const SDL_Color BACKGROUND_COLOR;

	static const int TILE_DROP_DELAY;
	static const int DELAY_CONTINUOUS_KEY;

	static mutex eventMutex;
	static SDL_TimerID autoSingleDropEvent;

	shared_ptr<FontObject> textHold, textNext, textScore, textScoring, textFooter;
	shared_ptr<RectObject> borderLeft, borderRight, backgroundRect, backgroundScoring;
	map<pair<int, int>, FunctionPointer> eventMap;
	vector<shared_ptr<TileObject>> boardTileObjects;

	void singleDropAndRender();
	void handleButtonArrowDown();
	void handleButtonArrowDownContinuous();
	void handleButtonArrowUp();
	void handleButtonArrowUpContinuous();
	void handleButtonArrowLeft();
	void handleButtonArrowLeftContinuous();
	void handleButtonArrowRight();
	void handleButtonArrowRightContinuous();
	void handleButtonSpace();
	void handleButtonZ();
	void handleButtonX();
	void handleButtonC();
	void handleButtonCtrl();
	void handleButtonShift();
	void handleButtonEscape();
	void handleButtonP();

	void updateViewBoard(Board &board);
	void updateViewScoring(int scoring);
	void updateViewHoldingTile(Tile &tile);
	void updateViewPreparingTile(vector<Tile> preparingTiles);
public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual void start(bool newGame);
    virtual void gameLoop(SDL_Event &event);
	static Uint32 autoSingleDrop(Uint32 interval, void *param);
};