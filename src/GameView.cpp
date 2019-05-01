#include "GameView.h"

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
	tileTextures(),
	fullTileTextures(),
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
}

void GameView::init() {
	initWindow();
	initRenderer();
	initImage();
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
}

void GameView::initImage() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) && imageFlags)) {
		throw Exception(IMG_GetError());
	}
}

shared_ptr<SDL_Texture> GameView::createTexture(const string &path) {
	shared_ptr<SDL_Surface> loadedSurface = createSurface(path);
	if (loadedSurface == nullptr) {
		throw Exception(IMG_GetError());
	}
	SDL_SetColorKey(loadedSurface.get(), SDL_RLEACCEL, loadedSurface->format->Amask);
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
	return loadedRawSurface;
}

void GameView::initTileTexture() {
	tileTextures[EMPTY] = createTexture(Tile(EMPTY).getAssetPath());
	tileTextures[SHADOW] = createTexture(Tile(SHADOW).getAssetPath());
	fullTileTextures[EMPTY] = createTexture(Tile(EMPTY).getAssetFullTilePath());
	for (const auto &tileType : Constants::MAP_TILE_TYPE) {
		tileTextures[tileType] = createTexture(Tile(tileType).getAssetPath());
		fullTileTextures[tileType] = createTexture(Tile(tileType).getAssetFullTilePath());
	}
}

void GameView::initTextureText() {
	if (TTF_Init() == -1) {
		throw Exception(TTF_GetError());
	}
}

shared_ptr<SDL_Texture> GameView::createTextureText(const string &text, int fontSize, SDL_Rect *rect, bool isBold) {
	shared_ptr<TTF_Font> font;
	if (isBold) {
		font = PointerDefinition::createTtfFont(
			TTF_OpenFont("assets/fonts/agency-fb-bold.ttf", fontSize)
		);
	} else {
		font = PointerDefinition::createTtfFont(
			TTF_OpenFont("assets/fonts/agency-fb.ttf", fontSize)
		);
	}
	shared_ptr<SDL_Surface> surfaceMessage = PointerDefinition::createSdlSurface(
		TTF_RenderText_Blended(font.get(), text.c_str(), colorWhite)
	);
	*rect = surfaceMessage->clip_rect;
	shared_ptr<SDL_Texture> textureMessage = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), surfaceMessage.get())
	);
	return textureMessage;
}

void GameView::renderDisplayObject(shared_ptr<DisplayObject> displayObject) {
	auto texture = createTexture(displayObject->path);
	SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &(displayObject->rect));
	SDL_RenderPresent(renderer.get());
}

void GameView::renderDisplayObject(vector<shared_ptr<DisplayObject>> displayObjects) {
	for (auto displayObject : displayObjects) {
		renderDisplayObject(displayObject);
	}
}

void GameView::renderFontObject(shared_ptr<FontObject> fontObject, int align) {
	int x = fontObject->rect.x;
	int y = fontObject->rect.y;
	auto texture = createTextureText(fontObject->text, fontObject->fontSize, &fontObject->rect, fontObject->isBold);
	fontObject->rect.x = x;
	fontObject->rect.y = y;
	if (align == 1) {
		fontObject->rect.x = x - fontObject->rect.w/2;
	} else if (align == 2) {
		fontObject->rect.x = x - fontObject->rect.w;
	}
	SDL_RenderCopy(renderer.get(), texture.get(), NULL, &fontObject->rect);
	fontObject->rect.x = x;
	fontObject->rect.y = y;
	SDL_RenderPresent(renderer.get());
}

void GameView::renderFontObject(vector<shared_ptr<FontObject>> fontObjects, int align) {
	for (auto fontObject : fontObjects) {
		renderFontObject(fontObject, align);
	}
}

void GameView::renderRectObject(shared_ptr<RectObject> rectObject) {
	auto color = rectObject->color;
	SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer.get(), &rectObject->rect);
	SDL_RenderPresent(renderer.get());
}

void GameView::renderRectObject(vector<shared_ptr<RectObject>> rectObjects) {
	for (auto rectObject : rectObjects) {
		renderRectObject(rectObject);
	}
}

void GameView::renderTileObject(shared_ptr<TileObject> tileObject) {
	auto tileTexture = tileTextures[tileObject->type];
	SDL_RenderCopy(renderer.get(), tileTexture.get(), nullptr, &tileObject->rect);
	SDL_RenderPresent(renderer.get());
}

void GameView::renderTileObject(vector<shared_ptr<TileObject>> tileObjects) {
	for (auto tileObject : tileObjects) {
		auto tileTexture = tileTextures[tileObject->type];
		SDL_RenderCopy(renderer.get(), tileTexture.get(), nullptr, &tileObject->rect);
	}
	SDL_RenderPresent(renderer.get());
}

void GameView::renderFullTileObject(shared_ptr<FullTileObject> fullTileObject) {
	auto texture = fullTileTextures[fullTileObject->type];
	SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &fullTileObject->rect);
	SDL_RenderPresent(renderer.get());
}

void GameView::renderFullTileObject(vector<shared_ptr<FullTileObject>> fullTileObjects) {
	for (auto fullTileObject : fullTileObjects) {
		auto texture = fullTileTextures[fullTileObject->type];
		SDL_RenderCopy(renderer.get(), texture.get(), nullptr, &fullTileObject->rect);
	}
	SDL_RenderPresent(renderer.get());
}

shared_ptr<SDL_Window> GameView::getWindow() {
	return window;
}