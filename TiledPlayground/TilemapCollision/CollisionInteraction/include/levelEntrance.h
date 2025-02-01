#ifndef LEVELENTRANCE_H
#define LEVELENTRANCE_H

#include <raylib.h>
#include <tmx.h>
#include "mapRender.h"
#include "player.h"
#include "collisionObject.h"


// funcs
bool LoadLevel( tmx_map *map, Camera2D *camera, Player *player );
void UpdateLevel( tmx_map *map, Camera2D *camera, Player *player );
void DrawLevel( Player *player, tmx_map *map, Camera2D *camera, RenderTexture2D *targetTexture );
void FreeLevel( tmx_map *map );
#endif
