#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

// modular functions
bool Init();
SDL_Texture *LoadTexture( char *path );
bool LoadMedia();
void Quit();

// global variables
SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL; // window renderer
SDL_Texture *globalTexture = NULL; // current displayed texture
const int screenWidth = 640;
const int screenHeight = 480;

int main( int argc, char *argv[] )
{
  if( !Init() )
    printf( "Failed to initialize program!\n" );
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
      SDL_RenderClear(globalRenderer);
      // render texture to screen
      SDL_RenderCopy(globalRenderer, globalTexture, NULL, NULL);
      // update screen
      SDL_RenderPresent(globalRenderer);
    }
  }

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
    globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    }
    else 
    {
      globalRenderer = SDL_CreateRenderer(globalWindow, -1, SDL_RENDERER_ACCELERATED );
      if( globalRenderer == NULL )
      {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
      }
      else 
      {
        SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

        int imgFlag = IMG_INIT_PNG;
        if( !(IMG_Init(imgFlag) & imgFlag) )
        {
          printf( "SDL_Image could not be initialized! SDL_Image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }
  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
  }

  return success;
}

SDL_Texture *LoadTexture( char *path )
{
  SDL_Texture *newTexture = NULL; // the texture to return

  newTexture = IMG_LoadTexture(globalRenderer, "assets/texture.png");
  if( newTexture == NULL )
    printf( "Texture could not be loaded! SDL_Image Error: %s\n", IMG_GetError() );

  return newTexture;
}

bool LoadMedia()
{
  bool success = true;

  // load texture
  globalTexture = LoadTexture("assets/texture.png");
  if( globalTexture == NULL )
  {
    printf( "Failed to load texture!\n" );
    success = false;
  }

  return success;
}

void Quit()
{
  // free loaded texture
  SDL_DestroyTexture(globalTexture);
  globalTexture = NULL;

  // destroy renderer and window
  SDL_DestroyRenderer(globalRenderer);
  SDL_DestroyWindow(globalWindow);
  
  // quit sdl subsystems
  IMG_Quit();
  SDL_Quit();
}
