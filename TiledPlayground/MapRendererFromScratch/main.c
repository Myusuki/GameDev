#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <tmx.h>
#include "include/mapRender.h"

// window dimensions
#define windowWidth 800
#define windowHeight 640

// tmx callback glob funcs
// load 
void* RaylibTexLoad( const char *path );
// free
void RaylibTexFree( void *ptr );

int main(void)
{
  SetConfigFlags(FLAG_VSYNC_HINT);
  SetExitKey(KEY_NULL);

  InitWindow(windowWidth, windowHeight, "Map Renderer From Scratch");
  SetTargetFPS(60);

  tmx_img_load_func = RaylibTexLoad;
  tmx_img_free_func = RaylibTexFree;

  tmx_map *map = tmx_load("../assets/GameBoyTest.tmx");
  if( map == NULL )
    tmx_perror("Failed to load GameBoyTest.tmx");

  Camera2D camera = { .target = (Vector2){ (float)(map->width * map->tile_width) / 2, (float)(map->height * map->tile_height) / 2}, 
                      .offset = (Vector2){ (float)windowWidth / 2, (float)windowHeight / 2}, .rotation = 0.0f, .zoom = 5.0 };
  Rectangle mapRect = {0, 0, (map->tile_width * map->width), (map->tile_height * map->height) };

  while( !WindowShouldClose() )
  {
    // input handling
    // movement
    Vector2 worldPos = GetScreenToWorld2D( (Vector2){ ( (float)windowWidth / 2), ( (float)windowHeight / 2) }, camera);

    Vector2 CameraCheckPos1 = GetScreenToWorld2D( (Vector2){0 , 0}, camera); // top left of the screen
    Vector2 CameraCheckPos2 = GetScreenToWorld2D( (Vector2){windowWidth, windowHeight}, camera); // bottom right of the screen
    printf( "Top Left X: %f, Y: %f\n", CameraCheckPos1.x, CameraCheckPos1.y );
    printf( "Bottom Right X: %f, Y: %f\n", CameraCheckPos2.x, CameraCheckPos2.y );

    // if( CheckCollisionPointRec(leftCameraCheckPos, mapRect) )
    if( CameraCheckPos1.x > 0)
    {
      if( IsKeyDown(KEY_LEFT) )
        camera.target.x -= 16;
    }
    // if( CheckCollisionPointRec(rightCameraCheckPos, mapRect) )
    if( CameraCheckPos2.x < (map->tile_width * map->width) )
    {
      if( IsKeyDown(KEY_RIGHT) )
        camera.target.x += 16;
    }
    // if( CheckCollisionPointRec(topCameraCheckPos, mapRect) )
    if( CameraCheckPos1.y > 0 )
    {
      if( IsKeyDown(KEY_UP) )
        camera.target.y -= 8;
    }
    // if( CheckCollisionPointRec(bottomCameraCheckPos, mapRect) )
    if( CameraCheckPos2.y < (map->tile_height * map->height))
    {
      if( IsKeyDown(KEY_DOWN) )
        camera.target.y += 8;
    }
    // reset
    if( IsKeyPressed(KEY_R) )
      camera.target = (Vector2){ (float)(map->tile_width * map->width) / 2, (float)(map->tile_height * map->height) / 2};


    // logic
    // Vector2 screenToWorldPos = GetWorldToScreen2D( (Vector2){0, 0}, camera);
    // printf( "X: %f, Y:%f\n", screenToWorldPos.x, screenToWorldPos.y );
    printf( "X: %f, Y: %f\n", worldPos.x, worldPos.y );
    BeginDrawing();
      BeginMode2D(camera);
        RenderMap(map);
      EndMode2D();
    EndDrawing();
  }
  CloseWindow();
  return 0;
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
