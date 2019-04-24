#include "GameView.h"
#include <iostream>

shared_ptr<GameView> GameView::instance(nullptr);

shared_ptr<GameView> GameView::getInstance() {
    if (instance == nullptr) {
        instance = make_shared<GameView>();
    }
    return instance;
}

GameView::GameView(): 
	window(nullptr),
	windowSurface(nullptr),
	renderer(nullptr),
	texture(nullptr),
	tileTextures(),
	scoringFont(nullptr),
	colorWhite({0xff, 0xff, 0xff, 0xff})
{

}

GameView::~GameView() {
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

void GameView::startSDL() {
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
	drawTextureText();
}

void GameView::init() {
	initWindow();
	initRenderer();
	initImage();
	initTexture();
	initTileTexture();
	initTextureText();
}

void GameView::initWindow() {
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
		throw Exception(SDL_GetError());
	}
	windowSurface = PointerDefinition::createSdlSurface(
		SDL_GetWindowSurface(window.get())
	);
}

void GameView::initRenderer() {
	renderer = PointerDefinition::createSdlRenderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
	);
	if (renderer == nullptr) {
		throw Exception(SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);
}

void GameView::initImage() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) && imageFlags)) {
		throw Exception(IMG_GetError());
	}
}

void GameView::initTexture() {
	texture = createTexture(Constants::BACKGROUND_IMG_PATH);
	if (texture == nullptr) {
		throw Exception(SDL_GetError());
	}
}

shared_ptr<SDL_Texture> GameView::createTexture(const string &path) {
	shared_ptr<SDL_Surface> loadedSurface = createSurface(path);
	if (loadedSurface == nullptr) {
		throw Exception(IMG_GetError());
	}
	SDL_SetColorKey(loadedSurface.get(), SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0xff, 0xff, 0xff, 0x00));
	shared_ptr<SDL_Texture> res = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get())
	);
	SDL_SetTextureBlendMode(res.get(), SDL_BLENDMODE_BLEND);
	return res;
}

shared_ptr<SDL_Surface> GameView::createSurface(const string &path) {
	shared_ptr<SDL_Surface> loadedRawSurface = PointerDefinition::createSdlSurface(
		IMG_Load(path.c_str())
	);
	shared_ptr<SDL_Surface> loadedSurface = PointerDefinition::createSdlSurface(
		SDL_ConvertSurface(loadedRawSurface.get(), windowSurface->format, 0)
	);
	return loadedSurface;
}

void GameView::initTileTexture() {
	tileTextures[EMPTY] = createTexture(Tile(EMPTY).getAssetPath());
	tileTextures[I] = createTexture(Tile(I).getAssetPath());
	tileTextures[J] = createTexture(Tile(J).getAssetPath());
	tileTextures[L] = createTexture(Tile(L).getAssetPath());
	tileTextures[O] = createTexture(Tile(O).getAssetPath());
	tileTextures[S] = createTexture(Tile(S).getAssetPath());
	tileTextures[T] = createTexture(Tile(T).getAssetPath());
	tileTextures[Z] = createTexture(Tile(Z).getAssetPath());
}

void GameView::initTextureText() {
	if (TTF_Init() == -1) {
		throw Exception(TTF_GetError());
	}

	scoringFont = PointerDefinition::createTtfFont(
		TTF_OpenFont("assets/fonts/agency-fb.ttf", 50)
	);
	TTF_SetFontStyle(scoringFont.get(), TTF_STYLE_BOLD);
}

shared_ptr<SDL_Texture> GameView::createTextureText(const string &text, int fontSize, SDL_Rect *rect) {
	auto font = PointerDefinition::createTtfFont(
		TTF_OpenFont("assets/fonts/agency-fb.ttf", fontSize)
	);
	shared_ptr<SDL_Surface> surfaceMessage = PointerDefinition::createSdlSurface(
		TTF_RenderText_Solid(font.get(), text.c_str(), colorWhite)
	);
	*rect = surfaceMessage->clip_rect;
	shared_ptr<SDL_Texture> textureMessage = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), surfaceMessage.get())
	);
	return textureMessage;
}

void GameView::drawBackground() {
	SDL_RenderClear(renderer.get());
	SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
	SDL_RenderPresent(renderer.get());
}

void GameView::drawTextureText() {
	drawTextureHold();
	drawTextureNext();
	drawTextureScore();
	drawTextureFooter();
	SDL_RenderPresent(renderer.get());
}

void GameView::drawTextureHold() {
	SDL_Rect rect;
	auto textureHold = createTextureText("hold", 36, &rect);
	rect.x = 121;
	rect.y = 41;
	SDL_RenderCopy(renderer.get(), textureHold.get(), NULL, &rect);
}

void GameView::drawTextureNext() {
	SDL_Rect rect;
	auto textureNext = createTextureText("next", 36, &rect);
	rect.x = 731;
	rect.y = 41;
	SDL_RenderCopy(renderer.get(), textureNext.get(), NULL, &rect);
}

void GameView::drawTextureScore() {
	SDL_Rect rect;
	auto textureScore = createTextureText("score", 36, &rect);
	rect.x = 112;
	rect.y = 298;
	SDL_RenderCopy(renderer.get(), textureScore.get(), NULL, &rect);
}

void GameView::drawTextureFooter() {
	SDL_Rect rect;
	auto textureFooter = createTextureText("press ESC or P to pause", 24, &rect);
	rect.x = 59;
	rect.y = 555;
	SDL_RenderCopy(renderer.get(), textureFooter.get(), NULL, &rect);
}

void GameView::updateBoard(Board &board) {
    for (int i = Constants::BOARD_HEIGHT/2; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
			copyTileToRenderer(&board[i][j]);
        }
    }
	SDL_RenderPresent(renderer.get());
}

void GameView::copyTileToRenderer(Tile *t) {
	auto tileTexture = tileTextures[t->getType()];
	SDL_Rect &rect = t->getPositionOnWindow();
	SDL_RenderCopy(renderer.get(), tileTexture.get(), nullptr, &rect);
}