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
#include <time.h>

// modular functions 
bool Init();
SDL_Texture* LoadTexture( char *path );
bool LoadMedia();
void Quit();

// global variables
SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL;
SDL_Texture *globalTexture = NULL;
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  // viewports
  SDL_Rect topLeftViewport = { 0, 0, screenWidth / 2, screenHeight / 2 };
  SDL_Rect topRightViewport = { screenWidth / 2, 0, screenWidth / 2, screenHeight / 2 };
  SDL_Rect bottomViewport = { 0, screenHeight / 2, screenWidth, screenHeight / 2 };
  // thick lines
  SDL_Rect verticalLine = { screenWidth / 2, 0, screenWidth / 100, screenHeight / 2 };
  SDL_Rect horizontalLine = { 0, screenHeight / 2, screenWidth, screenHeight / 100 };
  
  if( !Init() )
    printf( "Failed to initialize!\n" );
  else
  {
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
      // top left viewport
      SDL_RenderSetViewport(globalRenderer, &topLeftViewport);
      SDL_RenderCopy(globalRenderer, globalTexture, NULL, NULL);
      // top right viewport 
      SDL_RenderSetViewport(globalRenderer, &topRightViewport);
      SDL_RenderCopy(globalRenderer, globalTexture, NULL, NULL);
      // bottom viewport
      SDL_RenderSetViewport(globalRenderer, &bottomViewport);
      SDL_RenderCopy(globalRenderer, globalTexture, NULL, NULL);
      // draw viewport lines
      SDL_SetRenderDrawColor(globalRenderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderSetViewport(globalRenderer, NULL);
      // SDL_RenderFillRect(globalRenderer, &verticalLine);
      // SDL_RenderFillRect(globalRenderer, &horizontalLine);
      SDL_RenderDrawLine(globalRenderer, screenWidth / 2, 0, screenWidth / 2, screenHeight / 2);
      SDL_RenderDrawLine(globalRenderer, 0, screenHeight / 2, screenWidth, screenHeight / 2);

      SDL_RenderPresent(globalRenderer);
    }
  }
  
  Quit();
  return 0;
}

bool Init()
{
  bool success = true;
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf( "SDL could not be initialized! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
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
        int imgFlags = IMG_INIT_PNG;
        if( !(IMG_Init(imgFlags) & imgFlags) )
        {
          printf( "SDL_Image failed to initialize! SDL_Image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }

  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    success = false;
  }

  return success;
}

SDL_Texture* LoadTexture( char *path )
{
  SDL_Texture *texture = IMG_LoadTexture(globalRenderer, path);
  if( texture == NULL )
    printf( "%s failed to load! SDL_Image Error: %s\n", path, IMG_GetError() );

  return texture;
}

bool LoadMedia()
{
  bool success = true;
  globalTexture = LoadTexture("assets/viewport.png");
  if( globalTexture == NULL )
  {
    printf( "Failed to load texture!\n" );
    success = false;
  }

  return success;
}

void Quit()
{
  SDL_DestroyTexture(globalTexture);
  globalTexture = NULL;
  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
