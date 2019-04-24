#pragma once
#include <memory>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

class PointerDefinition {
private:
	PointerDefinition();
	virtual ~PointerDefinition();
public:
	static shared_ptr<SDL_Window> createSdlWindow(SDL_Window *p);
	static shared_ptr<SDL_Renderer> createSdlRenderer(SDL_Renderer *p);
	static shared_ptr<SDL_Surface> createSdlSurface(SDL_Surface *p);
	static shared_ptr<SDL_Texture> createSdlTexture(SDL_Texture *p);

	static shared_ptr<Mix_Music> createMixMusic(Mix_Music *p);
	static shared_ptr<Mix_Chunk> createMixChunk(Mix_Chunk *p);

	static shared_ptr<TTF_Font> createTtfFont(TTF_Font *p);
};