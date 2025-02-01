#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct Texture
{
  SDL_Texture *texture;
  int width;
  int height;
} Texture;

void TextureInit( Texture *pTexture );
void FreeTexture( Texture *pTexture );
bool LoadTextureFromFile( Texture *pTexture, char *path );
#if defined (SDL_TTF_MAJOR_VERSION)
bool LoadTextureFromRenderedText( Texture *pTexture, char *textureText, SDL_Color textColor );
#endif
void RenderTexture( Texture *pTexture, int x, int y, SDL_Rect *clipSpace );

#endif
