#include <stdio.h>
#include <raylib.h>
#include <tmx.h>

#define windowWidth 800
#define windowHeight 600

int main(void)
{
  // InitWindow(windowWidth, windowHeight, "Tiled Playground");
  // SetTargetFPS(60);

  tmx_map *map = tmx_load("./assets/CaveRuinsEntrance.tmx");
  char *formatVersion = map->format_version;
  char *classString = map->class_type; // NULL if not user defined or found in tmx file
  int orientation = map->orient; // enums default to 0 for the first enum member
  unsigned int mapWidth = map->width; // width is in cells not px
  unsigned int mapHeight = map->height; // height is in cells not px
  unsigned int tileWidth = map->tile_width;
  unsigned int tileHeight = map->tile_height;
  int mapStaggerIndex = map->stagger_index;
  int mapStaggerAxis = map->stagger_axis;
  unsigned int backgroundColor = map->backgroundcolor;
  int renderOrder = map->renderorder;
  tmx_properties *mapProperties = map->properties;
  tmx_tileset_list *tilesetHead = map->ts_head;
  tmx_layer *layerHead = map->ly_head;
  unsigned int mapTilecount = map->tilecount;
  tmx_tile **mapTiles = map->tiles;


  while( tilesetHead )
  {
    printf( "%s\n", tilesetHead->source);
    tmx_tileset *tileset = tilesetHead->tileset;
    printf( "Tileset Tilecount: %d\n", tileset->tilecount );
    // tmx_tile checkTile = tileset->tiles[57];
    // unsigned int checkTileCollision = checkTile.collision->obj_type;
    // printf( "Tile Collision Type: %d\n", checkTileCollision );

    tilesetHead = tilesetHead->next;
  }
  while( layerHead != NULL )
  {
    tmx_layer *layer = layerHead;
    // for( int row = 0; row < map->height; row++ )
    // {
    //   for( int column = 0; column < map->width; column++ )
    //   {
    //     int32_t cell = layer->content.gids[row * map->width + column];
    //     int32_t GID = cell & TMX_FLIP_BITS_REMOVAL;
    //     printf( "Column: %d, Row: %d\n", column, row );
    //   }
    // }
    int32_t cell = layer->content.gids[ 7 * map->width + 0 ];
    int32_t GID = cell & TMX_FLIP_BITS_REMOVAL;
    tmx_tile *tile = map->tiles[ GID ];
    printf( "Tile found!\n" );
    printf( "%s\n", layerHead->name );
    layerHead = layerHead->next;
  }
  printf("%d\n", mapTilecount );

  return 0;
}

