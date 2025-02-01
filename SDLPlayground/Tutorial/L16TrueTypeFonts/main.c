#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#include "lib/init.h"
#include "lib/texture.h"

// modular functions
bool Init();
bool LoadMedia();
void Quit();

Texture globalTextTexture;

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
    }

    SDL_SetRenderDrawColor(globalRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(globalRenderer);

    RenderTexture(&globalTextTexture, (screenWidth - globalTextTexture.width) / 2, (screenHeight - globalTextTexture.height) / 2, NULL);

    SDL_RenderPresent(globalRenderer);
  }
  Quit();

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
  globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED);
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
  if( TTF_Init() == -1 )
  {
    printf( "SDL_TTF failed to initialize! SDL_TTF Error: %s\n", TTF_GetError );
    return false;
  }

  TextureInit(&globalTextTexture);

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    return false;
  }

  return true;
}
bool LoadMedia()
{
  // open font
  globalFont = TTF_OpenFont("assets/lazy.ttf", 28);
  if( globalFont == NULL )
  {
    printf( "Failed to load lazy font! SDL_TTF Error: %s\n", TTF_GetError );
    return false;
  }
  // render text
  SDL_Color textColor = {.r = 0, .g = 0, .b = 0, .a = 0xff};
  if( !LoadTextureFromRenderedText(&globalTextTexture, "The quick brown fox jumps over the lazy dog!", textColor) )
  {
    printf( "Failed to render text texture!\n" );
    return false;
  }

  return true;
}
void Quit()
{
  FreeTexture(&globalTextTexture);

  // close font
  TTF_CloseFont(globalFont);
  globalFont = NULL;

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
