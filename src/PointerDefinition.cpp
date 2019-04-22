#include "PointerDefinition.h"

using namespace std;

PointerDefinition::PointerDefinition() {

}

PointerDefinition::~PointerDefinition() {

}

shared_ptr<SDL_Window> PointerDefinition::createSdlWindow(SDL_Window *p) {
	shared_ptr<SDL_Window> res(p, SDL_DestroyWindow);
	return res;
}

shared_ptr<SDL_Renderer> PointerDefinition::createSdlRenderer(SDL_Renderer *p) {
	shared_ptr<SDL_Renderer> res(p, SDL_DestroyRenderer);
	return res;
}

shared_ptr<SDL_Texture> PointerDefinition::createSdlTexture(SDL_Texture *p) {
	shared_ptr<SDL_Texture> res(p, SDL_DestroyTexture);
	return res;
}

shared_ptr<SDL_Surface> PointerDefinition::createSdlSurface(SDL_Surface *p) {
	shared_ptr<SDL_Surface> res(p, SDL_FreeSurface);
	return res;
}


shared_ptr<Mix_Music> PointerDefinition::createMixMusic(Mix_Music *p) {
	shared_ptr<Mix_Music> res(p, Mix_FreeMusic);
	return res;
}

shared_ptr<Mix_Chunk> PointerDefinition::createMixChunk(Mix_Chunk *p) {
	shared_ptr<Mix_Chunk> res(p, Mix_FreeChunk);
	return res;
}