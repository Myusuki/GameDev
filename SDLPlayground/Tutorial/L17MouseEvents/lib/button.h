#ifndef BUTTON_H
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL.h>
#include "init.h"
#include "texture.h"

// button constants
#define buttonWidth 300
#define buttonHeight 200
#define totalButtons 4

typedef struct Button
{
  // top-left position
  SDL_Point position;
  // currently used global sprite
  buttonSprites currentSprite;
} Button;

void ButtonInit( Button *pButton );
// set top left position
void SetButtonPosition( Button *pButton, int x, int y );
// handle mouse events
void HandleButtonEvent( Button *pButton, SDL_Event *event );
// shows button sprite
void RenderButton( Button *pButton, Texture *pTexture );
#endif
