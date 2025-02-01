#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <tmx.h>
#include "include/mapRender.h"

// macro funcs
#define MAX( a, b ) ( (a) > (b) ? (a) : (b) )
#define MIN( a, b ) ( (a) < (b) ? (a) : (b) )

// player
typedef struct Player
{
  Vector2 position;
  Rectangle hurtbox;
  Vector2 speed;
  bool canJump;
} Player;

// modular functions
void UpdatePlayer( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect, float deltaTime );
void UpdatePlayerCamera( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect );

int main(void)
{
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

  InitWindow(windowWidth, windowHeight, "Window Resizing");
  SetWindowMinSize(windowWidth, windowHeight);
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);

  tmx_img_load_func = RaylibTexLoad;
  tmx_img_free_func = RaylibTexFree;

  tmx_map *map = tmx_load("assets/GameBoyTest.tmx");
  if( map == NULL )
  {
    tmx_perror( "Could not load GameBoyTest.tmx" );
    return 1;
  }

  // render texture
  RenderTexture2D targetTexture = LoadRenderTexture(windowWidth, windowHeight);
  SetTextureFilter(targetTexture.texture, TEXTURE_FILTER_BILINEAR);

  Rectangle mapRect = { .x = 0, .y = 0, .width = map->width * map->tile_width, .height = map->height * map->tile_height };
  Player player = { .position = (Vector2){ mapRect.width / 2, mapRect.height / 2 },
                    .hurtbox = { player.position.x - 8, player.position.y - 8, 16, 16 },
                    .speed = (Vector2){ 128, 128 }, .canJump = false };
  Camera2D camera = { .target = player.position,
                      .offset = (Vector2){ (float)windowWidth / 2, (float)windowHeight / 2},
                      .rotation = 0.0f, .zoom = 5.0f };

  while( !WindowShouldClose() )
  {
    printf( "Screen Width: %d\n", GetScreenWidth() );
    printf( "Screen Height: %d\n", GetScreenHeight() );
    float scale = MIN( (float)GetScreenWidth() / windowWidth, (float)GetScreenHeight() / windowHeight );
    printf( "Scale Factor: %f\n", scale );

    UpdatePlayer(&camera, &player, windowWidth, windowHeight, mapRect, GetFrameTime() );

    // render tilemap to render texture
    BeginTextureMode(targetTexture);
      BeginMode2D(camera);
        RenderMap(map);
        DrawRectangle(player.position.x - 8, player.position.y - 8, 16, 16, BLACK);
        DrawPixel(player.position.x, player.position.y, WHITE);
        DrawPixel(player.hurtbox.x, player.hurtbox.y, RED);
      EndMode2D();
    EndTextureMode();
    // draw
    BeginDrawing();
      ClearBackground(BLACK);
      DrawTexturePro(targetTexture.texture, (Rectangle){0, 0, targetTexture.texture.width, -targetTexture.texture.height},
        (Rectangle){ (GetScreenWidth() - ( (float)windowWidth * scale) ) * 0.5f, (GetScreenHeight() - ( (float)windowHeight * scale) ) * 0.5f, (float)windowWidth * scale, (float)windowHeight * scale}, 
        (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
  }
  tmx_map_free(map);
  CloseWindow();

  return 0;
}

void UpdatePlayer( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect, float deltaTime )
{ 
  // player directional controls
  if( IsKeyDown(KEY_LEFT) )
  {
    if( player->speed.x > 0 ) // change speed direction
      player->speed.x *= -1;
    if( player->hurtbox.x > mapRect.x ) // bounds check
    {
      player->position.x += (int)(player->speed.x * deltaTime);
      player->hurtbox.x += (int)(player->speed.x * deltaTime);
    }
  }
  else if( IsKeyDown(KEY_RIGHT) )
  {
    if( player->speed.x < 0 )
      player->speed.x *= -1;
    if( (player->hurtbox.x + player->hurtbox.width) < mapRect.width ) // bounds check
    {
      player->position.x += (int)(player->speed.x * deltaTime);
      player->hurtbox.x += (int)(player->speed.x * deltaTime);
    }
  }
  else if( IsKeyDown(KEY_UP) )
  {
    if( player->speed.y > 0 )
      player->speed.y *= -1;
    if( player->hurtbox.y > mapRect.y ) // bounds check
    {
      player->position.y += (int)(player->speed.y * deltaTime);
      player->hurtbox.y += (int)(player->speed.y * deltaTime);
    }
  }
  else if( IsKeyDown(KEY_DOWN) )
  {
    if( player->speed.y < 0 )
      player->speed.y *= -1;
    if( (player->hurtbox.y + player->hurtbox.height) < mapRect.height ) // bounds check
    {
      player->position.y += (int)(player->speed.y * deltaTime);
      player->hurtbox.y += (int)(player->speed.y * deltaTime);
    }
  }

  // reset player to map center
  if( IsKeyPressed(KEY_R) )
  {
    player->position = (Vector2){ mapRect.width / 2, mapRect.height / 2 };
    player->hurtbox.x = player->position.x - 8;
    player->hurtbox.y = player->position.y - 8;
  }

  UpdatePlayerCamera(camera, player, windowWidth, windowHeight, mapRect);
}

void UpdatePlayerCamera( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect )
{
  // camera boundary check points
  Vector2 leftCheckPoint = GetScreenToWorld2D( (Vector2){ 0, screenHeight / 2.0f }, *camera);
  printf( "LEFT X: %f, Y: %f\n", leftCheckPoint.x, leftCheckPoint.y );
  Vector2 rightCheckPoint = GetScreenToWorld2D( (Vector2){ screenWidth, screenHeight / 2.0f }, *camera);
  printf( "RIGHT X: %f, Y: %f\n", rightCheckPoint.x, rightCheckPoint.y );
  Vector2 topCheckPoint = GetScreenToWorld2D( (Vector2){ screenWidth / 2.0f, 0 }, *camera);
  printf( "TOP X: %f, Y: %f\n", topCheckPoint.x, topCheckPoint.y );
  Vector2 bottomCheckPoint = GetScreenToWorld2D( (Vector2){ screenWidth / 2.0f, screenHeight }, *camera);
  printf( "BOTTOM X: %f, Y: %f\n", bottomCheckPoint.x, bottomCheckPoint.y );
  // Vector2 topLeftPoint = GetScreenToWorld2D( (Vector2){ 0, 0 }, *camera);
  // printf( "TOP LEFT X: %f, Y: %f\n", topLeftPoint.x, topLeftPoint.y );
  // Vector2 bottomRightPoint = GetScreenToWorld2D( (Vector2){ screenWidth, screenHeight }, *camera);
  // printf( "BOTTOM RIGHT X: %f, Y: %f\n", bottomRightPoint.x, bottomRightPoint.y );
  Vector2 centerPoint = GetScreenToWorld2D( (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }, *camera);
  printf( "Center Point\nX: %f, Y: %f\n", centerPoint.x, centerPoint.y );
  printf( "Map Dim. X: %f, Y: %f, W: %f, H: %f\n", mapRect.x, mapRect.y, mapRect.width, mapRect.height );

  printf( "PLAYER POS X: %f, Y: %f\n", player->position.x, player->position.y );
  
  if( leftCheckPoint.x > mapRect.x && player->position.x < centerPoint.x && player->speed.x < 0 )
    camera->target.x = player->position.x;
  if( rightCheckPoint.x < mapRect.width && player->position.x > centerPoint.x && player->speed.x > 0 )
    camera->target.x = player->position.x;
  if( topCheckPoint.y > mapRect.y && player->position.y < centerPoint.y && player->speed.y < 0 )
    camera->target.y = player->position.y;
  if( bottomCheckPoint.y < mapRect.height && player->position.y > centerPoint.y && player->speed.y > 0 )
    camera->target.y = player->position.y;
}
