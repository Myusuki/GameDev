#include <SDL2/SDL_error.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

// modular functions
bool Init();
bool LoadMedia();
SDL_Surface* LoadSurface( char *path );
void Quit();

// enum
enum KeyPressedSurfaces
{
  KEYPRESS_SURFACE_DEFAULT,
  KEYPRESS_SURFACE_UP,
  KEYPRESS_SURFACE_DOWN,
  KEYPRESS_SURFACE_LEFT,
  KEYPRESS_SURFACE_RIGHT,
  KEYPRESS_SURFACE_TOTAL
};

// global variables
SDL_Window *globalWindow = NULL;
SDL_Surface *globalSurface = NULL;
SDL_Surface *keyPressedSurfaces[KEYPRESS_SURFACE_TOTAL];
SDL_Surface *currentSurface = NULL;
const int screenWidth = 1280;
const int screenHeight = 960;

int main( int argc, char *argv[] )
{
  SDL_Rect stretchRect;
  stretchRect.x = 0;
  stretchRect.y = 0;
  stretchRect.w = screenWidth;
  stretchRect.h = screenHeight;

  if( !Init() )
  {
    printf( "Failed to initialize!" );
    exit(0);
  }
  bool quit = false;
  SDL_Event event;
  currentSurface = keyPressedSurfaces[KEYPRESS_SURFACE_DEFAULT];

  while( !quit )
  {
    while( SDL_PollEvent(&event) )
    {
      if( event.type == SDL_QUIT )
        quit = true;
      else if( event.type == SDL_KEYDOWN )
      {
        switch( event.key.keysym.sym )
        {
          case SDLK_UP:
            currentSurface = keyPressedSurfaces[KEYPRESS_SURFACE_UP];
            break;
          case SDLK_DOWN:
            currentSurface = keyPressedSurfaces[KEYPRESS_SURFACE_DOWN];
            break;
          case SDLK_LEFT:
            currentSurface = keyPressedSurfaces[KEYPRESS_SURFACE_LEFT];
            break;
          case SDLK_RIGHT:
            currentSurface = keyPressedSurfaces[KEYPRESS_SURFACE_RIGHT];
            break;
          default:
            currentSurface = keyPressedSurfaces[KEYPRESS_SURFACE_DEFAULT];
            break;
        }
      }
    }
    SDL_BlitScaled(currentSurface, NULL, globalSurface, NULL);
    SDL_UpdateWindowSurface(globalWindow);
  }

  Quit();

  return 0;
} 

bool Init()
{
  bool success = true;
  if( SDL_Init(SDL_INIT_VIDEO) > 0 )
  {
    printf( "Unable to initialize SDL! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Unable to create window! SDL Error:%s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      globalSurface = SDL_GetWindowSurface(globalWindow);
      if( globalSurface == NULL )
      {
        printf( "Unable to get window surface! SDL Error: %s\n", SDL_GetError() );
        success = false;
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

SDL_Surface* LoadSurface( char *path )
{
  // final optimized surface
  SDL_Surface *optimizedSurface = NULL;

  SDL_Surface *loadedSurface = SDL_LoadBMP(path);
  if( loadedSurface == NULL )
    printf( "Unable to load %s! SDL Error: %s\n", path, SDL_GetError() );
  else
  {
    optimizedSurface = SDL_ConvertSurface(loadedSurface, globalSurface -> format, 0);
    if( optimizedSurface == NULL )
      printf( "Unable to optimize surface! SDL Error: %s\n", SDL_GetError() );

    SDL_FreeSurface(loadedSurface);
  }

  return optimizedSurface;
}

bool LoadMedia()
{
  bool success = true;

  // load default surface
  keyPressedSurfaces[KEYPRESS_SURFACE_DEFAULT] = LoadSurface("assets/default.bmp");
  if( keyPressedSurfaces[KEYPRESS_SURFACE_DEFAULT] == NULL )
  {
    printf( "Failed to load default image!\n" );
    success = false;
  }
  // load up surface
  keyPressedSurfaces[KEYPRESS_SURFACE_UP] = LoadSurface("assets/up.bmp");
  if( keyPressedSurfaces[KEYPRESS_SURFACE_UP] == NULL )
  {
    printf( "Failed to load up image!\n" );
    success = false;
  }
  // load down surface
  keyPressedSurfaces[KEYPRESS_SURFACE_DOWN] = LoadSurface("assets/down.bmp");
  if( keyPressedSurfaces[KEYPRESS_SURFACE_DOWN] == NULL )
  {
printf( "Failed to load down image!\n" );
    success = false;
  }
  // load left image
  keyPressedSurfaces[KEYPRESS_SURFACE_LEFT] = LoadSurface("assets/left.bmp");
  if( keyPressedSurfaces[KEYPRESS_SURFACE_LEFT] == NULL )
  {
    printf( "Failed to load left image!\n" );
    success = false;
  }
  // load right image
  keyPressedSurfaces[KEYPRESS_SURFACE_RIGHT] = LoadSurface("assets/right.bmp");
  if( keyPressedSurfaces[KEYPRESS_SURFACE_RIGHT] == NULL )
  {
    printf( "Failed to load right image!\n" );
    success = false;
  }

  return success;
}

void Quit()
{
  for( int surfaces = 0; surfaces < KEYPRESS_SURFACE_TOTAL; surfaces++ )
  {
    SDL_FreeSurface(keyPressedSurfaces[surfaces] );
    keyPressedSurfaces[surfaces] = NULL;
  }

  SDL_DestroyWindow(globalWindow);

  SDL_Quit();
}
