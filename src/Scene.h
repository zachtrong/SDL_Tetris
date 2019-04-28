#pragma once
#include "GameView.h"
#include <map>
#include <memory>

using namespace std;

class Scene {
private:
public:
    static shared_ptr<GameView> view;
	static const SDL_Rect RECT_BACKGROUND;
    shared_ptr<DisplayObject> background;
    map<string, shared_ptr<DisplayObject> > displayObjects;

    Scene();
    virtual ~Scene();

    virtual void start();
    virtual void gameLoop(SDL_Event &event);
};