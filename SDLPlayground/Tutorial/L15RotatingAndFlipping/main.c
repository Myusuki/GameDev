#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "lib/init.h"
#include "lib/texture.h"

// modular function
bool Init();
bool LoadMedia();
void Quit();

// texture structs
Texture globalArrow;

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 1;
  }

  // rotating and flipping variables
  double angleInDegrees = 0;
  SDL_RendererFlip flipType = SDL_FLIP_NONE;

  bool quit = false;
  SDL_Event event;
  while( !quit )
  {
    while( SDL_PollEvent(&event) > 0 )
    {
      if( event.type == SDL_QUIT )
        quit = true;
      else if( event.type == SDL_KEYDOWN )
      {
        switch( event.key.keysym.sym )
        {
          case SDLK_a:
            angleInDegrees -= 60;
            break;
          case SDLK_d:
            angleInDegrees += 60;
            break;
          case SDLK_q:
            if( flipType == SDL_FLIP_HORIZONTAL )
              flipType = SDL_FLIP_NONE;
            else
              flipType = SDL_FLIP_HORIZONTAL;
            break;
          case SDLK_w:
            flipType = SDL_FLIP_NONE;
            break;
          case SDLK_e:
            if( flipType == SDL_FLIP_VERTICAL )
              flipType = SDL_FLIP_NONE;
            else
              flipType = SDL_FLIP_VERTICAL;
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(globalRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(globalRenderer);

    RenderTexture(&globalArrow, (screenWidth - globalArrow.width) / 2, (screenHeight - globalArrow.height) / 2, NULL, angleInDegrees, NULL, flipType);

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

  int imgFlags = IMG_INIT_PNG;
  if( !(IMG_Init(imgFlags) & imgFlags) )
  {
    printf( "SDL_Image failed to initialize! SDL_Image Error: %s\n", IMG_GetError );
    return false;
  }

  TextureInit(&globalArrow);

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    return false;
  }

  return true;
}

bool LoadMedia()
{
  if( !LoadTextureFromFile(&globalArrow, "assets/arrow.png") )
  {
    printf( "Failed to load arrow.png\n" );
    return false;
  }

  return true;
}

void Quit()
{
  FreeTexture(&globalArrow);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
