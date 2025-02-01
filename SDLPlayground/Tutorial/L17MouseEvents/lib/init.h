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

typedef enum buttonSprites {
  BUTTON_SPRITE_MOUSE_OUT = 0,
  BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
  BUTTON_SPRITE_MOUSE_DOWN = 2,
  BUTTON_SPRITE_MOUSE_UP = 3,
  BUTTON_SPRITE_MOUSE_TOTAL = 4
} buttonSprites;

SDL_Rect globalSpriteClips[BUTTON_SPRITE_MOUSE_TOTAL];

#endif
