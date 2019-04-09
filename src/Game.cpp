#include "Game.h"

using namespace std;

shared_ptr<Game> Game::instance(nullptr);

Game::Game(): 
	window(nullptr),
	renderer(nullptr),
	texture(nullptr) {

}

Game::~Game() {
	IMG_Quit();
	SDL_Quit();
}

shared_ptr<Game> Game::getInstance() {
	if (instance == nullptr) {
		instance = make_shared<Game>();
	}
	return instance;
}

void Game::start() {
	try {
		init();
	} catch (const Exception &e) {
		printf("Something went wrong!!! %s", e.what());
		return;
	} catch (const char* message) {
		printf("What a Terrible Failure, %s", message);
		return;
	}

	printf("initialization successful!\n");
	
	drawBackground();
	drawRedFilledQuad();
	processEvent();
}

void Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw new Exception(SDL_GetError());
		return;
	}
	initWindow();
	initRenderer();
	initImage();
	initTexture();
}

void Game::initWindow() {
	window = PointerDefinition::createSdlWindow(
		SDL_CreateWindow(
			Constants::GAME_TITLE, 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			Constants::SCREEN_WIDTH, 
			Constants::SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN
		)
	);
	if (window == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

void Game::initRenderer() {
	renderer = PointerDefinition::createSdlRenderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
	);
	if (renderer == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

void Game::initImage() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) && imageFlags)) {
		throw new Exception(IMG_GetError());
	}
}

void Game::initTexture() {
	texture = PointerDefinition::createSdlTexture(
		loadBackgroundTexture()
	);
	if (texture == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

SDL_Texture* Game::loadBackgroundTexture() {
	shared_ptr<SDL_Surface> loadedSurface(IMG_Load("assets/textures/background.png"));
	if (loadedSurface == nullptr) {
		throw new Exception(IMG_GetError());
	}
	return SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get());
}

void Game::drawBackground() {
	SDL_SetRenderDrawColor(renderer.get(), 0xff, 0xff, 0xff, 0xff);

	SDL_RenderClear(renderer.get());
	SDL_RenderCopy(renderer.get(), texture.get(), NULL, NULL);
	SDL_RenderPresent(renderer.get());
}

void Game::drawRedFilledQuad() {
	//Render red filled quad
	SDL_Rect fillRect = { 
		Constants::SCREEN_WIDTH / 4, 
		Constants::SCREEN_HEIGHT / 4, 
		Constants::SCREEN_WIDTH / 2, 
		Constants::SCREEN_HEIGHT / 2 
	};
	SDL_SetRenderDrawColor( renderer.get(), 0xFF, 0x00, 0x00, 0xFF );        
	SDL_RenderFillRect( renderer.get(), &fillRect );
	SDL_RenderPresent(renderer.get());
}

void Game::processEvent() {
	SDL_Event event;
	bool running = true;
	while (running) {
		SDL_WaitEvent(&event);
		if (event.type == SDL_QUIT)
			running = false;
	}
}