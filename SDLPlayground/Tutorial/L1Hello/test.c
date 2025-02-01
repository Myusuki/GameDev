#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  // init sdl
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
  
  // create window
  SDL_Window *window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
  if( window == NULL )
    printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
  else
  {
    // get surface
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    // fill surface with white rectangle
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface -> format, 0xff, 0xff, 0xff) );

    // update surface
    SDL_UpdateWindowSurface(window);

    // maintain window to stay open
    SDL_Event event;
    bool quit = false;
    while( !quit )
    {
      while( SDL_PollEvent(&event) )
      {
        if( event.type == SDL_QUIT )
          quit = true;
      }
    }
  }

  // destroy window 
  SDL_DestroyWindow(window);

  // quit sdl
  SDL_Quit();

  return 0;
}
