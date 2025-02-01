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
Texture globalModulatedTexture;

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 1;
  }

  bool quit = false;
  SDL_Event event;
  // color modulation values
  Uint8 red = 255;
  Uint8 green = 255;
  Uint8 blue = 255;

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
          // increase red
          case SDLK_q:
            red += 32;
            break;
          // increase green
          case SDLK_w:
            green += 32;
            break;
          // increase blue
          case SDLK_e:
            blue += 32;
            break;
          // decrease red
          case SDLK_a:
            red -= 32;
            break;
          // decrease green
          case SDLK_s:
            green -= 32;
            break;
          // decrease blue
          case SDLK_d:
            blue -= 32;
            break;
          // reset
          case SDLK_0:
            red = 255;
            green = 255;
            blue = 255;
            break;
        }
      }
    }
    
    SDL_SetRenderDrawColor(globalRenderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(globalRenderer);

    // modulate colors
    SetTextureColor(&globalModulatedTexture, red, green, blue);
    RenderTexture(&globalModulatedTexture, 0, 0, NULL);

    SDL_RenderPresent(globalRenderer);
  }
  Quit();

  return 0;
}

bool Init()
{
  if( SDL_Init(SDL_INIT_VIDEO) )
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

  TextureInit(&globalModulatedTexture);

  if( !LoadMedia() )
  {
    printf( "Failed to load media\n" );
    return false;
  }

  return true;
}

bool LoadMedia()
{
  if( !LoadTexture(&globalModulatedTexture, "assets/colors.png") )
  {
    printf( "Failed to load colors.png\n" );
    return false;
  }

  return true;
}

void Quit()
{
  FreeTexture(&globalModulatedTexture);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
