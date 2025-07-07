#include "mapRender.h"
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <tmx.h>

Color IntToColor( int color );
void DrawTile( void *image, Rectangle sourceRect, Vector2 destPos, float opacity, unsigned int flags );
void DrawTileCollision( tmx_tile *tile, Vector2 destPos );
void DrawTileLayer( tmx_map *map, tmx_layer *layer );

void RenderMap( tmx_map *map )
{
  ClearBackground(IntToColor(map->backgroundcolor) );

  tmx_layer *layerHead = map->ly_head;
  while( layerHead )
  {
    if( layerHead->type == L_LAYER )
      DrawTileLayer(map, layerHead);

    layerHead = layerHead->next;
  }
}
Color IntToColor( int color )
{
  tmx_col_floats colorValues = tmx_col_to_floats(color);
  return (Color){ colorValues.r, colorValues.g, colorValues.b, colorValues.a };
}

void DrawTile( void *image, Rectangle sourceRect, Vector2 destPos, float opacity, unsigned int flags )
{
  Texture2D *texture = (Texture2D*)image;

  int op = 0xFF * opacity;
  DrawTextureRec(*texture, sourceRect, destPos, (Color){op, op, op, op} );
}
void DrawTileCollision( tmx_tile *tile, Vector2 destPos )
{
  Rectangle destRect= { .x = destPos.x, .y = destPos.y, .width = tile->width, .height = tile->height };

  if( tile->collision )
    DrawRectangleRec(destRect, (Color){ 0, 255, 242, 107 } );
}
void DrawTileLayer( tmx_map *map, tmx_layer *layer )
{
  float opacity = layer->opacity;
  void* imagePtr;
  tmx_tileset *tileset;
  tmx_image *image;

  for( unsigned long rows = 0; rows < map->width; rows++ )
  {
    for( unsigned long columns = 0; columns < map->height; columns++ )
    {
      unsigned int gid = ( layer->content.gids[ (columns * map->width) + rows] ) & TMX_FLIP_BITS_REMOVAL;
      tmx_tile *tile = map->tiles[gid];

      if( tile != NULL )
      {
        tileset = tile->tileset;
        image = tile->image;
        unsigned int x = map->tiles[gid]->ul_x;
        unsigned int y = map->tiles[gid]->ul_y;
        unsigned int w = tileset->tile_width;
        unsigned int h = tileset->tile_height;

        // set tile image
        if( image )
          imagePtr = image->resource_image;
        else
          imagePtr = tileset->image->resource_image;

        unsigned int flags = ( layer->content.gids[ (columns * map->width) + rows ] ) & ~TMX_FLIP_BITS_REMOVAL;

        Rectangle sourceRectangle = { .x = x, .y = y, .width = w, .height = h };
        Vector2 destPosition = (Vector2){ rows * tileset->tile_width, columns * tileset->tile_height };
        DrawTile(imagePtr, sourceRectangle, destPosition, opacity, flags);
        DrawTileCollision( tile, destPosition );
        }
    }
  }
}

void* RaylibTexLoad( const char *path )
{
  Texture2D *texture = malloc( sizeof(Texture2D) );
  *texture = LoadTexture(path);
  return texture;
}
void RaylibTexFree( void *ptr )
{
  UnloadTexture( *( (Texture2D*)ptr) );
  free(ptr);
}
