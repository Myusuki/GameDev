#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

// modular functions
bool Init();
SDL_Surface* LoadSurface( char *path );
bool LoadMedia();
void Close();

// global variables
SDL_Window *globalWindow = NULL;
SDL_Surface *globalSurface = NULL;
// currently displayed png surface
SDL_Surface *globalPNGSurface = NULL;
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  if( !Init() )
    printf( "Failed to initialize!\n" );
  else 
  {
    if( !LoadMedia() )
      printf( "Failed to load media!\n" );
    else
    {
      bool quit = false;
      SDL_Event e;

      while( !quit )
      {
        while( SDL_PollEvent(&e) != 0 )
        {
          if( e.type == SDL_QUIT )
            quit = true;
        }
        SDL_BlitSurface( globalPNGSurface, NULL, globalSurface, NULL );
        SDL_UpdateWindowSurface(globalWindow);
      }
    }
  }

  Close();

  return 0;
}

bool Init()
{
  bool success = true;

  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf( "SDL could not be initialize! SDL Error: %s\n", SDL_GetError() );
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
      // init png loading
      int imgFlags = IMG_INIT_PNG;
      if( IMG_Init(imgFlags) != imgFlags )
      {
        printf( "SDL_Image could not initialize! SDL_Image Error: %s\n", IMG_GetError() );
        success = false;
      }
      else
        globalSurface = SDL_GetWindowSurface(globalWindow);
    }
  }

  return success;
}

SDL_Surface* LoadSurface( char *path )
{
  SDL_Surface *optimizedSurface = NULL;

  // load image
  SDL_Surface *loadedSurface = IMG_Load(path);
  if( loadedSurface == NULL )
    printf( "%s failed to load! SDL_Image Error: %s\n", path, IMG_GetError() );
  else
  {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, globalSurface -> format, 0);
    if( optimizedSurface == NULL )
      printf( "%s failed to optimize! SDL Error: %s\n", path, SDL_GetError() );

    SDL_FreeSurface(loadedSurface);
  }

  return optimizedSurface;
}

bool LoadMedia()
{
  bool success = true;

  globalPNGSurface = LoadSurface("assets/default.png");
  if( globalPNGSurface == NULL )
  {
    printf( "Failed to load PNG image!\n" );
    success = false;
  }

  return success;
}

void Close()
{
  SDL_FreeSurface(globalPNGSurface);
  globalPNGSurface = NULL;

  SDL_DestroyWindow(globalWindow);
  IMG_Quit();
  SDL_Quit();
}
