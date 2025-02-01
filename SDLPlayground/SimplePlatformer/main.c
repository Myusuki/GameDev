#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

// global variables
#define screenWidth 800
#define screenHeight 600

// app struct
typedef struct App 
{
  SDL_Window *window;
  SDL_Renderer *renderer;
} App;

// game struct
App Game = {0};

// global modular functions
bool Init();
void Quit();

int main( int argc, char *argv[] )
{
  // init program
  if( !Init() )
  {
    printf( "Failed to initialize!\n" );
    return 1;
  }

  bool quit = false;
  SDL_Event event;
  // timer
  Uint32 startTimer = 0;

  // game loop
  while( !quit )
  {
    // poll for input events
    while( SDL_PollEvent(&event) > 0 )
    {
      if( event.type == SDL_QUIT )
        quit = true;
      else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN )
        startTimer = SDL_GetTicks();
    }
    // draw stuff to screen
    SDL_RenderClear(Game.renderer);
    SDL_SetRenderDrawColor(Game.renderer, 0xff, 0xff, 0xff, 0xff);

    printf( "Milliseconds since start time: %d\n", SDL_GetTicks() - startTimer );

    SDL_RenderPresent(Game.renderer);
  }

  return 0;
}

bool Init()
{
  // init sdl
  if(SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf( "SDL failed to initialize! SDL Error: %s\n", SDL_GetError() );
    return false;
  }

  // init Game struct
  Game.window = SDL_CreateWindow("Simple Platformer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  if( Game.window == NULL )
  {
    printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }
  Game.renderer = SDL_CreateRenderer(Game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if( Game.renderer == NULL )
  {
    printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
    return false;
  }
  SDL_SetRenderDrawColor(Game.renderer, 0xff, 0xff, 0xff, 0xff);

  return true;
}
void Quit()
{
  // destroy renderer and window
  SDL_DestroyRenderer(Game.renderer);
  Game.renderer = NULL;
  SDL_DestroyWindow(Game.window);
  Game.window = NULL;

  // quit sdl
  SDL_Quit();
}
