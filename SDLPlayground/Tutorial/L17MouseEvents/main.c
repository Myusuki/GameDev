#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "lib/init.h"
#include "lib/texture.h"
#include "lib/button.h"

// modular functions
bool Init();
bool LoadMedia();
void Quit();

// mouse button sprites
Texture globalButtonSpriteSheetTexture;

// button objects
Button globalButtons[totalButtons];

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 1;
  }

  bool quit = false;
  SDL_Event event;
  while( !quit )
  {
    while( SDL_PollEvent(&event) > 0 )
    {
      if( event.type == SDL_QUIT )
        quit = true;
      // handle button events
      for( int button = 0; button < totalButtons; ++button )
        HandleButtonEvent(&globalButtons[button], &event);
    }
    SDL_SetRenderDrawColor(globalRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(globalRenderer);

    // render buttons
    for( int button = 0; button < totalButtons; button++ )
    {
      RenderButton(&globalButtons[button], &globalButtonSpriteSheetTexture);
    }

    SDL_RenderPresent(globalRenderer);
  }
  return 0;
}

bool Init()
{
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf( "SDL failed to initialize! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  if( globalWindow == NULL )
  {
    printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  if( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") )
    printf( "Warning: Linear texture filtering not enabled!\n");

  globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if( globalRenderer == NULL )
  {
    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }
  SDL_SetRenderDrawColor(globalRenderer, 0xff, 0xff, 0xff, 0xff);

  int imgFlags = IMG_INIT_PNG;
  if( !(IMG_Init(imgFlags) & imgFlags) )
  {
    printf( "SDL_Image failed to initialize! SDL_Image Error: %s\n", IMG_GetError );
    return false;
  }

  TextureInit(&globalButtonSpriteSheetTexture);
  // init buttons
  for( int button = 0; button < totalButtons; ++button )
    ButtonInit(&globalButtons[button]);

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n");
    return false;
  }

  return true;
}
bool LoadMedia()
{
  if( !LoadTextureFromFile(&globalButtonSpriteSheetTexture, "assets/button.png") )
  {
    printf( "Failed to load button.png!\n" );
    return false;
  }

  // set sprites
  for( int clip = 0; clip < totalButtons; ++clip )
  {
    globalSpriteClips[clip] = (SDL_Rect){ .x = 0, .y = clip * 200, .w = buttonWidth, .h = buttonHeight };
  }
  // set buttons in corners
  SetButtonPosition(&globalButtons[0], 0, 0);
  SetButtonPosition(&globalButtons[1], screenWidth - buttonWidth, 0);
  SetButtonPosition(&globalButtons[2], 0, screenHeight - buttonHeight);
  SetButtonPosition(&globalButtons[3], screenWidth - buttonWidth, screenHeight - buttonHeight);

  return true;
}
void Quit()
{
  FreeTexture(&globalButtonSpriteSheetTexture);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
