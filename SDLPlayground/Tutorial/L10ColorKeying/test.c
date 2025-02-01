#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>

// global variables
SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL;
const int screenWidth = 640;
const int screenHeight = 480;
// texture struct
typedef struct Texture
{
  SDL_Texture *texture;
  int width;
  int height;
} Texture;
Texture fooTexture, backgroundTexture;
// texture functions
void TextureInit( Texture *texture)
{
  texture->texture = NULL;
  texture->width = 0;
  texture->height = 0;
}
void FreeTexture( Texture *texture )
{
  if( texture->texture != NULL )
  {
    SDL_DestroyTexture(texture->texture);
    texture->width = 0;
    texture->height = 0;
  }
}
bool LoadTextureFromFile( Texture *texture, char *path )
{
  // free texture if it exists
  FreeTexture(texture);
  // create texture
  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path);
  if( loadedSurface == NULL )
    printf( "%s could not be loaded! SDL_Image Error: %s\n", path, IMG_GetError() );
  else
  {
    // set color key
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF) );
    newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
    if( newTexture == NULL )
      printf( "%s could not be converted to texture! SDL Error: %s\n", path, SDL_GetError() );
    else
    {
      texture->width = loadedSurface->w;
      texture->height = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  texture->texture = newTexture;
  return texture->texture != NULL;
}
void RenderTexture( Texture *texture, int x, int y )
{
  // set render space and render to screen
  SDL_Rect renderSpace = { x, y, texture->width, texture->height };
  SDL_RenderCopy(globalRenderer, texture->texture, NULL, &renderSpace);
}
// modular functions
bool Init();
bool LoadMedia();
void Quit();

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
      SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      SDL_RenderClear(globalRenderer);

      RenderTexture(&backgroundTexture, 0, 0);
      RenderTexture(&fooTexture, 240, 190);

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
    printf( "SDL could not initialize! SDL Error:%s\n", SDL_GetError() );
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
      else
      {
        SDL_SetRenderDrawColor(globalRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        int imgFlags = IMG_INIT_PNG;
        if( !(IMG_Init(imgFlags) & imgFlags) )
        {
          printf( "SDL_Image could not be initialized! SDL_Image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }

  TextureInit(&fooTexture);
  TextureInit(&backgroundTexture);
  
  if( !LoadMedia() )
  {
    printf( "Failed to load media!\n" );
    success = false;
  }

  return success;
}

bool LoadMedia()
{
  bool success = true;
  if( !LoadTextureFromFile(&fooTexture, "assets/foo.png") )
  {
    printf( "Failed to load foo.png texture!\n" );
    success = false;
  }

  if( !LoadTextureFromFile(&backgroundTexture, "assets/background.png") )
  {
    printf( "Failed to load background.png texture!\n" );
    success = false;
  }
  return success;
}

void Quit()
{
  FreeTexture(&fooTexture);
  FreeTexture(&backgroundTexture);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

