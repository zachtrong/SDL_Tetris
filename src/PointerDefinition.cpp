#include "PointerDefinition.h"

using namespace std;

PointerDefinition::PointerDefinition() {

}

PointerDefinition::~PointerDefinition() {

}

shared_ptr<SDL_Window> PointerDefinition::createSdlWindowPointer(SDL_Window *p) {
	shared_ptr<SDL_Window> res(p, SDL_DestroyWindow);
	return res;
}

shared_ptr<SDL_Renderer> PointerDefinition::createSdlRendererPointer(SDL_Renderer *p) {
	shared_ptr<SDL_Renderer> res(p, SDL_DestroyRenderer);
	return res;
}

shared_ptr<SDL_Texture> PointerDefinition::createSdlTexturePointer(SDL_Texture *p) {
	shared_ptr<SDL_Texture> res(p, SDL_DestroyTexture);
	return res;
}

shared_ptr<SDL_Surface> PointerDefinition::createSdlSurfacePointer(SDL_Surface *p) {
	shared_ptr<SDL_Surface> res(p, SDL_FreeSurface);
	return res;
}