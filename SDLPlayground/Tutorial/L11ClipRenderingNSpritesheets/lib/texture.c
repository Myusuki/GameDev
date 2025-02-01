#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "texture.h"
#include "init.h"
#include <SDL2/SDL_image.h>
#include <time.h>

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
  if( loadedSurface == NULL)
  {
    printf( "%s failed to load! SDL_Image Error: %s\n", path, IMG_GetError() );
    return false;
  }
  SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0, 0, 0, 0) );
  newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
  if( newTexture == NULL )
  {
    printf( "%s could not be converted to a texture! SDL Error: %s\n", path, SDL_GetError() );
    return false;
  }
  pTexture->width = loadedSurface->w;
  pTexture->height = loadedSurface->h;
  pTexture->texture = newTexture;

  return pTexture->texture != NULL;
}
void RenderTexture( Texture *pTexture, int x, int y, SDL_Rect *clipSpace )
{
  assert(pTexture);
  assert(clipSpace);

  SDL_Rect renderSpace = { x, y, pTexture->width, pTexture->height };

  if( clipSpace != NULL )
  {
    renderSpace.w = clipSpace->w;
    renderSpace.h = clipSpace->h;
  }
  SDL_RenderCopy(globalRenderer, pTexture->texture, clipSpace, &renderSpace);
}
