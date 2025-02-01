#ifndef INIT_H
#define INIT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

extern SDL_Window *globalWindow;
extern SDL_Renderer *globalRenderer;
extern TTF_Font *globalFont;
extern const int screenWidth;
extern const int screenHeight;

#endif
