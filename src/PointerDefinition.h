#include <memory>
#include <SDL.h>

#pragma once
using namespace std;

class PointerDefinition {
private:
	PointerDefinition();
	virtual ~PointerDefinition();
public:
	static shared_ptr<SDL_Window> createSdlWindow(SDL_Window *p);
	static shared_ptr<SDL_Renderer> createSdlRenderer(SDL_Renderer *p);
	/*
		depricated: use SDL_Texture instead of SDL_Surface
	*/
	static shared_ptr<SDL_Surface> createSdlSurface(SDL_Surface *p);
	static shared_ptr<SDL_Texture> createSdlTexture(SDL_Texture *p);
};