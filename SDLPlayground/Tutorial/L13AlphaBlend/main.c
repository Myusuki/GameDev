#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "lib/init.h"
#include "lib/texture.h"

// modular functions
bool Init();
bool LoadMedia();
void Quit();

// texture structs
Texture globalBackgroundTexture, globalModulatedTexture;

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 1;
  }

  bool quit = false;
  Uint8 alpha = 255;

  while( !quit )
  {
    SDL_Event event;
    while( SDL_PollEvent(&event) > 0 )
    {
      switch( event.type )
      {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          switch( event.key.keysym.sym )
          {
            case SDLK_w:
              if( alpha + 32 > 255 )
                alpha = 255;
              else
                alpha += 32;
              break;
            case SDLK_s:
              if( alpha - 32 < 0 )
                alpha = 0;
              else
                alpha -= 32;
              break;
          }
      }
    }
    SDL_SetRenderDrawColor(globalRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(globalRenderer);

    // render background
    RenderTexture(&globalBackgroundTexture, 0, 0, NULL);
    // render front texture
    SetTextureAlpha(&globalModulatedTexture, alpha);
    RenderTexture(&globalModulatedTexture, 0, 0, NULL);

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
    printf( "SDL_Image failed to initialize! SDL_Image Error: %s\n", IMG_GetError() );
    return false;
  }

  TextureInit(&globalBackgroundTexture);
  TextureInit(&globalModulatedTexture);

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    return false;
  }
  return true;
}

bool LoadMedia()
{
  if( !LoadTextureFromFile(&globalModulatedTexture, "assets/fadeout.png") )
  {
    printf( "Failed to load fadeout.png!\n" );
    return false;
  }
  else 
    SetTextureBlendMode(&globalModulatedTexture, SDL_BLENDMODE_BLEND);

  if( !LoadTextureFromFile(&globalBackgroundTexture, "assets/fadein.png") )
  {
    printf( "Failed to load fadein.png!\n" );
    return false;
  }

  return true;
}

void Quit()
{
  FreeTexture(&globalModulatedTexture);
  FreeTexture(&globalBackgroundTexture);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
