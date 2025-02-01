#include "mapRender.h"

void DrawTile( void *image, Rectangle sourceRect, Vector2 destPos, float opacity, unsigned int flags)
{
  Texture2D *texture = (Texture2D*)image;

  int op = 0xFF * opacity;
  DrawTextureRec(*texture, sourceRect, destPos, (Color){op, op, op, op} );
}
void DrawTileLayer( tmx_map *map, tmx_layer *layer )
{
  float opacity = layer->opacity;
  void* imagePtr;
  tmx_tileset *tileset;
  tmx_image *image;

  for( unsigned long columns = 0; columns < map->height; columns++ )
  {
    for( unsigned long rows = 0; rows < map->width; rows++ )
    {
      unsigned int gid = ( layer->content.gids[ (columns * map->width) + rows] ) &TMX_FLIP_BITS_REMOVAL;

      if( map->tiles[gid] != NULL )
      {
        tileset = map->tiles[gid]->tileset;
        image = map->tiles[gid]->image;
        unsigned int x = map->tiles[gid]->ul_x;
        unsigned int y = map->tiles[gid]->ul_y;
        unsigned int w = tileset->tile_width;
        unsigned int h = tileset->tile_height;

        if( image )
          imagePtr = image->resource_image;
        else
          imagePtr = tileset->image->resource_image;

        unsigned int flags = ( layer->content.gids[ (columns * map->width) + rows ]) & ~TMX_FLIP_BITS_REMOVAL;

        Rectangle sourceRectangle = { .x = x, .y = y, .width = w, .height = h };
        Vector2 destPosition = (Vector2){ rows * tileset->tile_width, columns * tileset->tile_height };
        DrawTile(imagePtr, sourceRectangle, destPosition, opacity, flags);
      }
    }
  }
}

Color IntToColor( int color )
{
  tmx_col_floats colorValues = tmx_col_to_floats(color);
  return (Color){ colorValues.r, colorValues.g, colorValues.b, colorValues.a };
}
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
