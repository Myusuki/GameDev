#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstddef>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <time.h>

// classes
class Texture{
  private:
    SDL_Texture *texture;
    int width;
    int height;
  public:
    Texture(); // constructor
    bool LoadFromFile( std::string path );
    void FreeTexture(); // deallocate texture
    void RenderTexture( int x, int y );
    int GetWidth();
    int GetHeight();
};

// global variables
SDL_Window *globalWindow = NULL;
SDL_Renderer *globalRenderer = NULL;
Texture fooTexture = Texture(), backgroundTexture = Texture();
const int screenWidth = 640;
const int screenHeight = 480;
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

      backgroundTexture.RenderTexture(0, 0);
      fooTexture.RenderTexture(240, 190);

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
  if( !fooTexture.LoadFromFile("assets/foo.png") )
  {
    printf( "Failed to load foo.png texture!\n" );
    success = false;
  }

  if( !backgroundTexture.LoadFromFile("assets/background.png") )
  {
    printf( "Failed to load background.png texture!\n" );
    success = false;
  }
  return success;
}

void Quit()
{
  fooTexture.FreeTexture();
  backgroundTexture.FreeTexture();

  SDL_DestroyRenderer(globalRenderer);
  globalRenderer = NULL;
  SDL_DestroyWindow(globalWindow);
  globalWindow = NULL;

  IMG_Quit();
  SDL_Quit();
}

Texture::Texture()
{
  texture = NULL;
  width = 0;
  height = 0;
}
bool Texture::LoadFromFile( std::string path )
{
  // free existing texture
  FreeTexture();
  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path.c_str() );
  if( loadedSurface == NULL )
    printf( "%s failed to load! SDL_Image Error: %s\n", path.c_str(), SDL_GetError() );
  else
  {
    // set color key
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface -> format, 0, 0xFF, 0xFF) );
    newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
    if( newTexture == NULL )
      printf( "%s could not be converted to a texture! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    else
    {
      width = loadedSurface->w;
      height = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  texture = newTexture;
  return texture != NULL;
}
void Texture::RenderTexture( int x, int y )
{
  // set rendering space and render to screen
  SDL_Rect renderSpace = { x, y, width, height };
  SDL_RenderCopy(globalRenderer, texture, NULL, &renderSpace);
}
int Texture::GetWidth()
{
  return width;
}
int Texture::GetHeight()
{
  return height;
}
void Texture::FreeTexture()
{
  // free texture if it exists
  if( texture != NULL )
  {
    SDL_DestroyTexture(texture);
    width = 0;
    height = 0;
  }
}
