#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include "init.h"
#include "texture.h"

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
    printf( "%s failed to load! SDL_Image Error: %s\n", path, IMG_GetError() );
    return false;
  }
  SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xff, 0xff) );

  newTexture = SDL_CreateTextureFromSurface(globalRenderer, loadedSurface);
  if( newTexture == NULL )
  {
    printf( "%s could not be converted to a texture! SDL Error: %s\n", path, SDL_GetError() );
    return false;
  }

  pTexture->width = loadedSurface->w;
  pTexture->height = loadedSurface->h;
  pTexture->texture = newTexture;

  SDL_FreeSurface(loadedSurface);

  return pTexture->texture != NULL;
}
void SetTextureColor( Texture *pTexture, Uint8 red, Uint8 green, Uint8 blue )
{
  assert(pTexture);
  SDL_SetTextureColorMod(pTexture->texture, red, green, blue);
}
void SetTextureBlendMode( Texture *pTexture, SDL_BlendMode blending )
{
  assert(pTexture);
  SDL_SetTextureBlendMode(pTexture->texture, blending);
}
void SetTextureAlpha( Texture *pTexture, Uint8 alpha )
{
  assert(pTexture);
  SDL_SetTextureAlphaMod(pTexture->texture, alpha);
}
void RenderTexture( Texture *pTexture, int x, int y, SDL_Rect *clipSpace )
{
  assert(pTexture);

  SDL_Rect renderSpace = { x, y, pTexture->width, pTexture->height };

  if( clipSpace != NULL )
  {
    renderSpace.w = clipSpace->w;
    renderSpace.h = clipSpace->h;
  }

  SDL_RenderCopy(globalRenderer, pTexture->texture, clipSpace, &renderSpace);
}
