#include <stdio.h>
#include <raylib.h>
#include <tmx.h>
#include "include/mapRender.h"
#include "include/levelEntrance.h"

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

int main(void)
{
  // screen dimensions
  const int windowWidth = 800;
  const int windowHeight = 600;

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetWindowSize(windowWidth, windowHeight);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Collision Interaction");
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);

  // tmx init related stuff
  tmx_img_load_func = RaylibTexLoad;
  tmx_img_free_func = RaylibTexFree;
  tmx_map *map = tmx_load("../../assets/CaveRuinsEntrance.tmx");
  if( map == NULL )
    tmx_perror( "Could not load CaveRuinsEntrance.tmx" );
  Rectangle mapRect = { .x = 0, .y = 0, .width= map->tile_width * map->width, .height = map->tile_height * map->height };

  // player init 
  Player player = { .position = { mapRect.width / 2, mapRect.height / 2 },
                    .hurtbox = { player.position.x - 8, player.position.y - 8, 16, 16 },
                    .speed = {128, 128} };
  // camera init
  Camera2D camera = { .target = player.position, .offset ={ windowWidth / 2.0f, windowHeight / 2.0f },
                      .rotation = 0.0f, .zoom = 5.0f }; 
  // render texture init
  RenderTexture2D targetTexture = LoadRenderTexture(windowWidth, windowHeight);
  SetTextureFilter( targetTexture.texture, TEXTURE_FILTER_BILINEAR);

  while( !WindowShouldClose() )
  {
    float scale = MIN(GetScreenWidth() / windowWidth, GetScreenHeight() / windowHeight);
    // logic and input
    UpdatePlayer(map, &camera, &player, (Vector2){ windowWidth, windowHeight }, GetFrameTime() );

    // draw
    // draw to render texture
    BeginTextureMode(targetTexture);
      BeginMode2D(camera);
        RenderMap(map);
        DrawPlayer(&player, map);
      EndMode2D();
    EndTextureMode();
    // draw to screen
    BeginDrawing();
      ClearBackground(BLACK);
      DrawTexturePro(targetTexture.texture, (Rectangle){ 0, 0, targetTexture.texture.width, -targetTexture.texture.height },
                    (Rectangle){ (GetScreenWidth() - ( (float)windowWidth * scale) ) * 0.5f, (GetScreenHeight() - ( (float)windowHeight * scale) ) * 0.5f, (float)windowWidth * scale, (float)windowHeight * scale }, 
                    (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
  }

  return 0;
}
