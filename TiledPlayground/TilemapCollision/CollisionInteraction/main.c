#include <stdio.h>
#include <raylib.h>
#include <tmx.h>
#include "include/mapRender.h"
#include "include/quadtree.h"
#include "include/player.h"

int main(void)
{
  const int windowWidth = 800;
  const int windowHeight = 600;

  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(windowWidth, windowHeight, "Collision Interaction");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  tmx_img_load_func = RaylibTexLoad;
  tmx_img_free_func = RaylibTexFree;

  tmx_map *map = tmx_load("../../assets/CaveRuinsEntrance.tmx");
  if( map == NULL )
  {
    tmx_perror( "Could not load CaveRuinsEntrance.tmx" );
    return 1;
  }

  // map rectangle
  Rectangle mapRect = { .x = 0, .y = 0, .width = (map->tile_width * map->width), .height = (map->tile_height * map->height) };
  // player
  Player player = { .position = (Vector2){ (mapRect.width / 2), (mapRect.height / 2) },
                   .hurtbox = (Rectangle){ (player.position.x - (map->tile_width / 2.0f) ), (player.position.y - (map->tile_height / 2.0f) ), map->tile_width, map->tile_height },
                   .speed = (Vector2){ 128, 128 } };
  // camera
  Camera2D camera = { .target = player.position,
                      .offset = (Vector2){ (windowWidth / 2.0f), (windowHeight / 2.0f) },
                      .rotation = 0.0f, .zoom = 5.0f };

	TreeNode rootNode;
	InitQuadTree(&rootNode, map);

  while( !WindowShouldClose() )
  {
    printf( "Map Tiles: %d\n", map->tilecount );
    printf( "Map Width: %d\n", map->width );
    printf( "Map Height: %d\n", map->height );
    // input handling and logic
    printf( "Screen Width: %d\n", GetScreenWidth() );
    printf( "Screen Height: %d\n", GetScreenHeight() );

    UpdatePlayer(mapRect, &camera, &player, (Vector2){ windowWidth, windowHeight}, GetFrameTime() );

    // draw
    BeginDrawing();
      BeginMode2D(camera);
        RenderMap(map);
        DrawPlayer(player);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
