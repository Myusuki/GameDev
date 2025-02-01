#include <SDL2/SDL.h>
#include "init.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL;
const int screenWidth = 640;
const int screenHeight = 480;
