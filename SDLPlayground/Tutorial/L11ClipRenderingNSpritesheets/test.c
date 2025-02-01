#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "lib/init.h"
#include "lib/texture.h"

// structs
Texture globalSpriteSheet;
SDL_Rect globalSpriteClips[4];

// modular functions
bool LoadMedia();
void Quit();

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 0;
  }
  
  TextureInit(&globalSpriteSheet);
  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    return 0;
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
    SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(globalRenderer);

    RenderTexture(&globalSpriteSheet, 0, 0, &globalSpriteClips[0] );
    RenderTexture(&globalSpriteSheet, screenWidth - 100, 0, &globalSpriteClips[1] );
    RenderTexture(&globalSpriteSheet, 0, screenHeight - 100, &globalSpriteClips[2] );
    RenderTexture(&globalSpriteSheet, screenWidth - 100, screenHeight - 100, &globalSpriteClips[3] );

    SDL_RenderPresent(globalRenderer);
  }

  Quit();
  return 0;
}

bool Init()
{
  bool success = true;
  if( SDL_Init(SDL_INIT_VIDEO) )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Window could not be created! SDL Error:%s\n", SDL_GetError() );
      success = false;
    }
    else 
    {
      globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED);
      if( globalRenderer == NULL )
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        int imgFlag = IMG_INIT_PNG;
        if( !(IMG_Init(imgFlag) & imgFlag) )
        {
          printf( "SDL Image could not be initialized! SDL_Image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }

  TextureInit(&globalSpriteSheet);

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    success = false;
  }

  return success;
}

bool LoadMedia()
{
  bool success = true;
  if( !LoadTexture(&globalSpriteSheet, "assets/dots.png") )
  {
    printf( "Failed to load dots.png\n" );
    success = false;
  }
  else
  {
    globalSpriteClips[0] = (SDL_Rect){ .x = 0, .y = 0, .w = 100, .h = 100 };
    globalSpriteClips[1] = (SDL_Rect){ .x = 100, .y = 0, .w = 100, .h = 100 };
    globalSpriteClips[2] = (SDL_Rect){ .x = 0, .y = 100, .w = 100, .h = 100 };
    globalSpriteClips[3] = (SDL_Rect){ .x = 100, .y = 100, .w = 100, .h = 100 };
  }

  return success;
}
void Quit()
{
  FreeTexture(&globalSpriteSheet);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
