#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

// init function
bool Init();
// load media
bool LoadMedia();
// free all media and close sdl
void Quit();

// window
SDL_Window *globalWindow = NULL;
// surface
SDL_Surface *globalSurface = NULL;
// image to render to globalSurface
SDL_Surface *helloWorld = NULL;
// window dimensions
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  // init
  if( !Init() )
    printf( "Failed to init!\n" );
  else
  {
    // load media
    if( !LoadMedia() )
      printf( "Failed to load media!\n" );
    else
    {
      // apply image
      SDL_BlitSurface( helloWorld, NULL, globalSurface, NULL );

      // update surface
      SDL_UpdateWindowSurface(globalWindow);

      // maintain window open
      SDL_Event event;
      bool quit = false;
      while( quit == false )
      {
        while( SDL_PollEvent(&event) )
        {
          if( event.type == SDL_QUIT )
            quit = true;
        }
      }
    }
  }

  // quit 
  Quit();

  return 0;
}

bool Init()
{
  // success flag
  bool success = true;

  if( SDL_Init(SDL_INIT_VIDEO) )
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
  else
  {
    // create window
    globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      globalSurface = SDL_GetWindowSurface(globalWindow);
    }
  }

  return success;
}
bool LoadMedia()
{
  // success flag
  bool success = true;

  // load splash image
  helloWorld = SDL_LoadBMP( "assets/HelloWorld.bmp" );
  if( helloWorld == NULL )
  {
    printf( "Could not load hello world image! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }

  return success;
}

void Quit()
{
  // deallocate surface
  SDL_FreeSurface(helloWorld);

  // destroy window
  SDL_DestroyWindow(globalWindow);

  // quit sdl
  SDL_Quit();
}
