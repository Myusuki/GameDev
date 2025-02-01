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
#include "init.h"
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
Texture globalSpriteSheetTexture;
SDL_Rect globalSpriteClips[4];
// texture functions
void TextureInit( Texture *pTexture )
{
  assert(pTexture);
  pTexture->texture = NULL;
  pTexture->width = 0;
  pTexture->height = 0;
}
void FreeTexture( Texture *pTexture )
{
  assert(pTexture);
  // if texture exists
  if( pTexture->texture != NULL )
  {
    SDL_DestroyTexture(pTexture->texture);
    pTexture->width = 0;
    pTexture->height = 0;
  }
}
bool LoadTexture( Texture *pTexture, char *path )
{
  assert(pTexture);
  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path);
  if( loadedSurface == NULL )
    printf( "%s failed to load! SDL_Image Error: %s\n", path, IMG_GetError() );
  else
  {
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF) );
    newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
    if( newTexture == NULL)
      printf( "%s could not be converted to texture! SDL Error: %s\n", path, SDL_GetError() );
    else
    {
      pTexture->width = loadedSurface->w;
      pTexture->height = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  pTexture->texture = newTexture;
  return pTexture->texture != NULL;
}
void RenderTexture( Texture *pTexture, int x, int y, SDL_Rect *clip )
{
  assert(pTexture);
  assert(clip);
  SDL_Rect renderSpace = { x, y, pTexture->width, pTexture->height };
  if( clip != NULL )
  {
    renderSpace.w = clip->w;
    renderSpace.h = clip->h;
  }
  SDL_RenderCopy(globalRenderer, pTexture->texture, clip, &renderSpace);
}

//modular functions
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

      RenderTexture(&globalSpriteSheetTexture, 0, 0, &globalSpriteClips[0] ); // top left
      RenderTexture(&globalSpriteSheetTexture, screenWidth - globalSpriteClips[1].w, 0, &globalSpriteClips[1] ); // top right
      RenderTexture(&globalSpriteSheetTexture, 0, screenHeight - globalSpriteClips[2].h, &globalSpriteClips[2] ); // bottom left
      RenderTexture(&globalSpriteSheetTexture, screenWidth - globalSpriteClips[3].w, screenHeight - globalSpriteClips[3].h, &globalSpriteClips[3] ); // bottom right

      SDL_RenderPresent(globalRenderer);
    }
  }
  return 0;
}

bool Init()
{
  bool success = true;
  if( SDL_Init(SDL_INIT_VIDEO) )
  {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  }
  else
  {
    globalWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if( globalWindow == NULL )
    {
      printf( "Window could not be created! SDL Error:%s\n", SDL_GetError() );
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
        int imgFlag = IMG_INIT_PNG;
        if( !(IMG_Init(imgFlag) & imgFlag) )
        {
          printf( "SDL Image could not be initialized! SDL_Image Error: %s\n", IMG_GetError() );
          success = false;
        }
      }
    }
  }

  TextureInit(&globalSpriteSheetTexture);

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

  if( !LoadTexture(&globalSpriteSheetTexture, "assets/dots.png") )
  {
    printf( "Failed to load dots.png\n" );
    success = false;
  }
  else
  {
    // setup sprite clips
    // top left
    globalSpriteClips[0] = (SDL_Rect){ .x = 0, .y = 0, .w = 100, .h = 100 };
    // top right
    globalSpriteClips[1] = (SDL_Rect){ .x = 100, .y = 0, .w = 100, .h = 100 };
    // bottom left 
    globalSpriteClips[2] = (SDL_Rect){ .x = 0, .y = 100, .w = 100, .h = 100 };
    // bottom right 
    globalSpriteClips[3] = (SDL_Rect){ .x = 100, .y = 100, .w = 100, .h = 100 };
  }

  return success;
}

void Quit()
{
  FreeTexture(&globalSpriteSheetTexture);

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}
