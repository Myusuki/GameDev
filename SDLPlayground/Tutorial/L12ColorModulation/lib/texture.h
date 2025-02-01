#ifndef TEXTURE_H
#define TEXUTRE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

typedef struct Texture
{
  SDL_Texture *texture;
  int width;
  int height;
} Texture;

void TextureInit( Texture *pTexture );
void FreeTexture( Texture *pTexture );
bool LoadTexture( Texture *pTexture, char *path );
void SetTextureColor( Texture *pTexture, Uint8 red, Uint8 green, Uint8 blue );
void RenderTexture( Texture *pTexture, int x, int y, SDL_Rect *clipSpace );

#endif
