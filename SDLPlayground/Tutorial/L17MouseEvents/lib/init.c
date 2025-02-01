#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <time.h>
#include "init.h"

SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL;
TTF_Font *globalFont = NULL;
const int screenWidth = 640;
const int screenHeight = 480;

