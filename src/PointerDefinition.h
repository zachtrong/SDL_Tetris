#include <memory>
#include <SDL.h>

#pragma once
using namespace std;

class PointerDefinition {
private:
  PointerDefinition();
  virtual ~PointerDefinition();
public:
  static shared_ptr<SDL_Window> createSdlWindowPointer(SDL_Window *p);
  static shared_ptr<SDL_Renderer> createSdlRendererPointer(SDL_Renderer *p);
  static shared_ptr<SDL_Surface> createSdlSurfacePointer(SDL_Surface *p);
  static shared_ptr<SDL_Texture> createSdlTexturePointer(SDL_Texture *p);
};