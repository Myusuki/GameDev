#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "button.h"
#include "init.h"
#include "texture.h"

void ButtonInit( Button *pButton )
{
  pButton->position = (SDL_Point){ .x = 0, .y = 0 };
  pButton->currentSprite = BUTTON_SPRITE_MOUSE_OUT;
}
void SetButtonPosition( Button *pButton, int x, int y )
{
  pButton->position = (SDL_Point){ .x = x, .y = y };
}
void HandleButtonEvent( Button *pButton, SDL_Event *event)
{
  // if mouse event occurred
  if( event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP )
  {
    // get mouse position
    int x, y;
    SDL_GetMouseState(&x, &y);

    // check if mouse is in button
    bool inside = true;

    // mouse is left of button
    if( x < pButton->position.x )
      inside = false;
    // mouse is right of button
    else if( x > pButton->position.x + buttonWidth )
      inside = false;
    // mouse is above button
    else if( y < pButton->position.y )
      inside = false;
    else if( y > pButton->position.y + buttonHeight )
      inside = false;

    // mouse is outside button
    if( !inside )
      pButton->currentSprite = BUTTON_SPRITE_MOUSE_OUT;
    // mouse is inside button
    else
    {
      switch( event->type )
      {
        case SDL_MOUSEMOTION:
          pButton->currentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
          break;
        case SDL_MOUSEBUTTONDOWN:
          pButton->currentSprite = BUTTON_SPRITE_MOUSE_DOWN;
          break;
        case SDL_MOUSEBUTTONUP:
          pButton->currentSprite = BUTTON_SPRITE_MOUSE_UP;
          break;
      }
    }
  }
}
void RenderButton(Button *pButton, Texture *pTexture )
{
  RenderTexture(pTexture, pButton->position.x, pButton->position.y, &globalSpriteClips[pButton->currentSprite]);
}
