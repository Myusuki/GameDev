#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{

  // window to render to 
  SDL_Window* window = NULL;

  // surface to render to 
  SDL_Surface* surface = NULL;

  // init sdl
  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
  else
  {
    // create window
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( window == NULL )
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
    else
    {
      // get window surface
      surface = SDL_GetWindowSurface(window);

      // fill the surface with white
      SDL_FillRect(surface, NULL, SDL_MapRGB(surface -> format, 0xFF, 0xFF, 0xFF) );

      // update the surface
      SDL_UpdateWindowSurface(window);

      // hack to get the window to stay
      SDL_Event e;
      bool quit = false;
      while( quit == false )
      {
        while( SDL_PollEvent(&e) )
        {
          if( e.type == SDL_QUIT )
            quit = true;
        }
      }
    }
  }

  // destroy window
  SDL_DestroyWindow(window);

  // quit sdl
  SDL_Quit();

  return 0;
}
