#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>

// modular functions 
bool Init();
void Quit();

// global variables
SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL;
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
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
      // clear screen
      SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(globalRenderer);
      // filled rectangle
      SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0x00, 0x00, 0xFF);
      SDL_Rect fillRect = { screenWidth / 4, screenHeight / 4, screenWidth / 2, screenHeight / 2 };
      SDL_RenderFillRect(globalRenderer, &fillRect);
      // outlined rectangle with empty center
      SDL_SetRenderDrawColor(globalRenderer, 0x00, 0xFF, 0x00, 0xFF);
      SDL_Rect outlineRect = { screenWidth / 6, screenHeight / 6, screenWidth * 2 / 3, screenHeight * 2 / 3 };
      SDL_RenderDrawRect(globalRenderer, &outlineRect);
      // blue horizontal line
      SDL_SetRenderDrawColor(globalRenderer, 0x00, 0x00, 0xFF, 0xFF);
      SDL_RenderDrawLine(globalRenderer, 0, screenHeight / 2, screenWidth, screenHeight / 2);
      // dotted yellow line
      SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0x00, 0xFF);
      for( int dot = 0; dot < screenHeight; dot += 4 )
        SDL_RenderDrawPoint(globalRenderer, screenWidth / 2, dot);
      
      // update screen
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
    printf( "SDL could not be initialized! SDL Error:%s\n", SDL_GetError() );
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
    }
  }

  return success;
}

void Quit()
{
  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  SDL_Quit();
}
