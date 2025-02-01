#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <raylib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <time.h>

// modular functions
bool Init();
bool LoadMedia();
SDL_Surface *LoadSurface( char *path );
void Quit();

// enum
typedef enum GameScreen
{
  LOGO, TITLE, ENDING, TOTAL
} GameScreen;

// global variables
SDL_Window *globalWindow = NULL;
SDL_Surface *globalSurface = NULL;
SDL_Surface *currentSurface = NULL;
SDL_Surface *screens[TOTAL];
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  if( !Init() )
  {
    printf( "Failed to initialize program!\n" );
    exit(0);
  }

  bool quit = false;
  GameScreen screen = LOGO;
  int framesCounter = 0;
  SDL_Event event;

  while( !quit )
  {
    switch( screen )
    {
      case LOGO:
        currentSurface = screens[LOGO];
        framesCounter++;
        
        if( framesCounter == 600 )
        {
          framesCounter = 0;
          screen = TITLE;
        }
        break;
      case TITLE:
        currentSurface = screens[TITLE];
        while( SDL_PollEvent(&event) > 0 )
        {
          if( event.type == SDL_QUIT )
          {
            quit = true;
          }
          else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN )
          {
            screen = ENDING;
          }
        }
        break;
      case ENDING:
        currentSurface = screens[ENDING];
        while( SDL_PollEvent(&event) > 0 )
        {
          if( event.type == SDL_QUIT )
            quit = true;
        }
        framesCounter++;
        if( framesCounter == 10000 )
          quit = true;
        break;

      default:
        break;
    }
    SDL_BlitSurface( currentSurface, NULL, globalSurface, NULL );
    SDL_UpdateWindowSurface(globalWindow);
  }

  Quit();

  return 0;
}

bool Init()
{
  bool success = true;

  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    globalWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else
    {
      globalSurface = SDL_GetWindowSurface(globalWindow);
      if( globalSurface == NULL )
      {
        printf( "Window surface could not be retreived! SDL Error: %s\n", SDL_GetError() );
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

SDL_Surface *LoadSurface( char *path )
{
  SDL_Surface *loadedSurface = SDL_LoadBMP(path);
  if( loadedSurface == NULL )
    printf( "%s failed to load! SDL Error: %s\n", path, SDL_GetError() );

  return loadedSurface;
}

bool LoadMedia()
{
  bool success = true;
  
  screens[LOGO] = LoadSurface("assets/logo.bmp");
  if( screens[LOGO] == NULL )
  {
    printf( "Failed to load Logo!\n" );
    success = false;
  }
  screens[TITLE] = LoadSurface("assets/title.bmp");
  if( screens[TITLE] == NULL )
  {
    printf( "Failed to load Title!\n" );
    success = false;
  }
  screens[ENDING] = LoadSurface("assets/ending.bmp");
  if( screens[ENDING] == NULL )
  {
    printf( "Failed to load ending!\n" );
    success = false;
  }

  return success;
}

void Quit()
{
  for( int surface = 0; surface < TOTAL; surface++ )
  {
    SDL_FreeSurface(screens[surface] );
    screens[surface] = NULL;
  }

  SDL_DestroyWindow(globalWindow);
  SDL_Quit();
}
