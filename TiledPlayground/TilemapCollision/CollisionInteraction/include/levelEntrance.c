#include "levelEntrance.h"
#include "player.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <tmx.h>

// func declarations
bool SetCollision( tmx_map *map, CollisionObj *collisionList );
int GetCollisionAmount( tmx_map *map );
int32_t GetGID( tmx_layer *layer, unsigned int mapWidth, unsigned int column, unsigned int row );
int GetCollisionTileType( tmx_tile *tile );

// file varaibles
int collisionObjAmount;
CollisionObj entranceCollisionList[] = { 0 };

bool LoadLevel( tmx_map *map, Camera2D *camera, Player *player )
{
  map = tmx_load("../../assets/CaveRuinsEntrance.tmx");
  if( map == NULL )
  {
    tmx_perror("Failed to load CaveRuinsEntrance.tmx");
    return false;
  }

  bool loadCollisionSuccess = SetCollision(map, entranceCollisionList);
  if( !loadCollisionSuccess )
  {
    printf( "Failed to load collision objects\n" );
    return false;
  }

  player->position = (Vector2){ map->width*map->tile_width / 2.0f, map->height*map->tile_height / 2.0f };
  camera->target = player->position;

  return true;
}
void UpdateLevel( tmx_map *map, Camera2D *camera, Player *player )
{
  // load map
  map = tmx_load("../../../assets/CaveRuinsEntrance.tmx");

  // update player
  Vector2 screenDimensions = { GetScreenWidth(), GetScreenHeight() };
  UpdatePlayer(map, camera, player, entranceCollisionList, screenDimensions, GetFrameTime() );
}
void DrawLevel( Player *player, tmx_map *map, Camera2D *camera, RenderTexture2D *targetTexture )
{
  BeginTextureMode(*targetTexture);
    BeginMode2D(*camera);
    EndMode2D();
  EndTextureMode();
}
void FreeLevel( tmx_map *map )
{
  tmx_map_free(map);
}

bool SetCollision( tmx_map *map, CollisionObj *collisionList )
{
  collisionObjAmount = GetCollisionAmount(map);
  if( collisionObjAmount == 0 )
    return true;
  int listIndex = 0;
  tmx_layer *layer = map->ly_head;
  if( layer == NULL )
  {
    printf("%s is NULL\n", layer->name );
    return false;
  }
  while( layer )
  {
    for( int row = 0; row < map->height; row++ )
    {
      for( int column = 0; column < map->width; column++ )
      {
        int gid = GetGID(layer, map->width, column, row);
        tmx_tile *tile = map->tiles[gid];
        if( tile == NULL )
        {
          printf( "Tile at %d, %d is NULL\n", column, row );
          return false;
        }
        if( tile->collision && listIndex < collisionObjAmount )
        {
          CollisionObj newObj;
          newObj.collisionType = GetCollisionTileType(tile);
          switch( newObj.collisionType )
          {
            case BLANK_TILE:
              break;
            case WALL:
              newObj.collisionBox = (Rectangle){ .x = column * map->tile_width, .y = row * map->tile_height,
                                                 .width = tile->width, .height = tile->height };
              break;
          }
          collisionList[listIndex] = newObj;
          listIndex++;
        }
      }
    }
  }
  return true;
}
int GetCollisionAmount( tmx_map *map )
{
  int collisionAmount = 0;
  tmx_layer *layer = map->ly_head;
  while( layer )
  {
    for( int row = 0; row < map->height; row++ )
    {
      for (int column = 0; column < map->width; column++ )
      {
        int gid = GetGID(layer, map->width, column, row);
        tmx_tile *tile = map->tiles[gid];
        if( tile->collision )
        {
          collisionAmount++;
        }
      }
    }
  }
  return collisionAmount;
}

// utility funcs
int32_t GetGID( tmx_layer *layer, unsigned int mapWidth, unsigned int column, unsigned int row )
{
  return layer->content.gids[ row * mapWidth + column ];
}
int GetCollisionTileType( tmx_tile *tile )
{
  int tileType = 0;
  int checkPropertyType = tmx_get_property(tile->properties, "uTileType")->type;
  int tilePropertyValue = tmx_get_property(tile->properties, "uTileType")->value.integer;
  if( checkPropertyType == PT_INT )
  {
    switch( tilePropertyValue )
    {
      case BLANK_TILE:
        tileType = 0;
        break;
      case WALL:
        tileType = 1;
        break;
    }
  }
  return tileType;
}
