#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "lib/init.h"
#include "lib/texture.h"

#define walkingAnimationFrames 4

// modular functions
bool Init();
bool LoadMedia();
void Quit();

// animation
SDL_Rect globalSpriteClips[walkingAnimationFrames];
Texture globalSpriteSheetTexture;

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 1;
  }

  // current frame of animation
  int frame = 0;
  
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

    SDL_Rect *currentClip = &globalSpriteClips[frame / 4]; // change animation frame every 4 frames
    RenderTexture(&globalSpriteSheetTexture, (screenWidth - currentClip->w) / 2, (screenHeight - currentClip->h) / 2, currentClip);

    SDL_RenderPresent(globalRenderer);

    ++frame;

    if( frame / 4 >= walkingAnimationFrames )
      frame = 0;
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
    printf( "SDL_Image failed to initialize! SDL Error: %s\n", IMG_GetError );
    return false;
  }

  TextureInit(&globalSpriteSheetTexture);

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    return false;
  }

  return true;
}
bool LoadMedia()
{
  if( !LoadTextureFromFile( &globalSpriteSheetTexture, "assets/foo.png") )
  {
    printf( "Failed to load foo.png\n" );
    return false;
  }

  // set sprite clips
  globalSpriteClips[0] = (SDL_Rect){ .x = 0, .y = 0, .w = 64, .h = 205 };
  globalSpriteClips[1] = (SDL_Rect){ .x = 64, .y = 0, .w = 64, .h = 205 };
  globalSpriteClips[2] = (SDL_Rect){ .x = 128, .y = 0, .w = 64, .h = 205 };
  globalSpriteClips[3] = (SDL_Rect){ .x = 192, .y = 0, .w = 64, .h = 205 };

  return true;
}
void Quit()
{
  FreeTexture(&globalSpriteSheetTexture);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
