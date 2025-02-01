#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

// modular functions
bool Init();
bool LoadMedia();
SDL_Surface* LoadSurface( char* path );
void Quit();

// Enum
enum KeyPressSurfaces
{
  KEYPRESS_SURFACE_DEFAULT,
  KEYPRESS_SURFACE_UP,
  KEYPRESS_SURFACE_DOWN,
  KEYPRESS_SURFACE_LEFT,
  KEYPRESS_SURFACE_RIGHT,
  KEYPRESS_SURFACE_TOTAL
};

// global variables
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
// image surfaces that correspond to key presses
SDL_Surface *keyPressedSurfaces[KEYPRESS_SURFACE_TOTAL];
SDL_Surface *currentSurface = NULL;
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  if( !Init() )
    printf( "Failed to initialize!" );
  else
  {
    if( !LoadMedia() )
      printf( "Failed to load media" );
    else
    {
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
        SDL_BlitSurface(currentSurface, NULL, surface, NULL);
        SDL_UpdateWindowSurface(window);
      }
    }
  }

  Quit();

  return 0;
} 

bool Init()
{
  bool success = true;

  if( SDL_Init(SDL_INIT_VIDEO) )
  {
    printf( "SDL unable to initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( window == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
      surface = SDL_GetWindowSurface(window);
  }

  return success;
}

SDL_Surface* LoadSurface( char* path )
{
  // load image 
  SDL_Surface* loadSurface = SDL_LoadBMP(path);
  if( loadSurface == NULL )
    printf( "%s could not be loaded! SDL Error: %s\n", path, SDL_GetError() );

  return loadSurface;
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
    SDL_FreeSurface(keyPressedSurfaces[surfaces]);
    keyPressedSurfaces[surfaces] = NULL;
  }

  SDL_DestroyWindow(window);

  SDL_Quit();
}
