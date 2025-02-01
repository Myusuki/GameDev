#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "init.h"
#include "texture.h"

void TextureInit( Texture *pTexture )
{
  pTexture->texture = NULL;
  pTexture->width = 0;
  pTexture->height = 0;
}
void FreeTexture( Texture *pTexture )
{
  assert(pTexture);

  SDL_DestroyTexture(pTexture->texture);
  pTexture->width = 0;
  pTexture->height = 0;
}
bool LoadTextureFromFile( Texture *pTexture, char *path )
{
  assert(pTexture);

  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path);
  if( loadedSurface == NULL )
  {
    printf( "%s failed to load! SDL_Image Error: %s\n", path, IMG_GetError );
    return false;
  }

  SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff) );

  newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
  if( newTexture == NULL )
  {
    printf( "%s failed to convert to a texture! SDL Error: %s\n", path, SDL_GetError() );
    return false;
  }
  pTexture->texture = newTexture;
  pTexture->width = loadedSurface->w;
  pTexture->height = loadedSurface->h;

  SDL_FreeSurface(loadedSurface);

  return pTexture->texture != NULL;
}
void RenderTexture( Texture *pTexture, int x, int y, SDL_Rect *clipSapce, double angle, SDL_Point *center, SDL_RendererFlip flip )
{
  assert(pTexture);

  // destination rectangle to render to
  SDL_Rect renderSpace = { x, y, pTexture->width, pTexture->height };
  // change render space to the size of clip space
  if( clipSapce != NULL )
  {
    renderSpace.w = clipSapce->w;
    renderSpace.h = clipSapce->h;
  }

  SDL_RenderCopyEx(globalRenderer, pTexture->texture, clipSapce, &renderSpace, angle, center, flip);
}
