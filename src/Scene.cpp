#include "Scene.h"

shared_ptr<GameView> Scene::view = GameView::getInstance();
shared_ptr<GameController> Scene::controller = GameController::getInstance();
shared_ptr<GameSound> Scene::sound = GameSound::getInstance();

const SDL_Rect Scene::RECT_BACKGROUND = {
	0, 0, 
	Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT
};

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::start() {

}

void Scene::gameLoop(SDL_Event &event) {

}