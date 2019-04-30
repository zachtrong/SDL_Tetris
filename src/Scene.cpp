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
	if (event.type == SDL_MOUSEMOTION) {
		handleMouse([](Button button){
			if (button.displayObject != nullptr) {
				view->renderDisplayObject(button.displayObject);
			}
		});
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		handleMouse([](Button button){
			if (button.callback != nullptr) {
				button.callback();
			}
		});
	}
}

void Scene::handleMouse(function<void (Button button)> callback) {
	bool anyInside = false;
	for (size_t i = 0; i < buttons.size(); ++i) {
		auto button = buttons[i];
		if (isMouseOverRect(button.displayObject->rect)) {
			anyInside = true;
			if (!togglingButtonStates[i]) {
				togglingButtonStates[i] = true;
				callback(button);
			}
		} else {
			togglingButtonStates[i] = false;
		}
	}
	if (!anyInside && !togglingButtonDefaultState) {
		togglingButtonDefaultState = true;
		callback(buttonDefault);
	}
}

bool Scene::isMouseOverRect(const SDL_Rect &rect) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return (
		rect.x <= mouseX
		&& mouseX <= rect.x + rect.w
		&& rect.y <= mouseY
		&& mouseY <= rect.y + rect.h
	);
}

void Scene::addButton(shared_ptr<DisplayObject> buttonView, function<void ()> onMouseClick) {
	buttons.push_back(Button(buttonView, onMouseClick));
    togglingButtonStates.push_back(false);
}

void Scene::setButtonDefault(shared_ptr<DisplayObject> buttonView, function<void ()> onMouseClick) {
	buttonDefault = Button(buttonView, onMouseClick);
	togglingButtonDefaultState = false;
}

void Scene::clearButton() {
	buttons.clear();
	togglingButtonStates.clear();
	buttonDefault = Button();
	togglingButtonDefaultState = false;
}