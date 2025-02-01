#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <tmx.h>
#include "include/mapRender.h"

// macro funcs
#define MAX( a, b ) ( (a) > (b) ? (a) : (b) )
#define MIN( a, b ) ( (a) < (b) ? (a) : (b) )

// tmx callback glob funcs
void* RaylibTexLoad( const char *path );
void RaylibTexFree( void *texture );
// modular funcs
bool CheckForFractionPortion( float number );
int DeleteFractionPortion( float number );

int main(void)
{
  // window dimensions
  const int windowWidth = 1024;
  const int windowHeight = 768;
  // game dimensions
  const int gameWidth = 800;
  const int gameHeight = 600;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);

  SetWindowSize(windowWidth, windowHeight);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Window Resizing");
  SetWindowMinSize(gameWidth, gameHeight);
  SetExitKey(KEY_NULL);
  SetTargetFPS(60);

  tmx_img_load_func = RaylibTexLoad;
  tmx_img_free_func = RaylibTexFree;

  tmx_map *map = tmx_load("../assets/CaveRuinsEntrance.tmx");
  if( map == NULL )
  {
    tmx_perror( "Could not load GameBoyTest.tmx" );
    return 1;
  }

  // render texture
  RenderTexture2D targetTexture = LoadRenderTexture(gameWidth, gameHeight);
  SetTextureFilter(targetTexture.texture, TEXTURE_FILTER_BILINEAR);

  Rectangle mapRect = { .x = 0, .y = 0, .width = map->width * map->tile_width, .height = map->height * map->tile_height };
  Camera2D camera = { .target = (Vector2){ mapRect.width / 2, mapRect.height / 2 },
                      .offset = (Vector2){ (float)gameWidth / 2, (float)gameHeight / 2}, .rotation = 0.0f, .zoom = 5.0f };
  int cameraSpd = 128;
  
  while( !WindowShouldClose() )
  {
    // window resizing scale factor
    float scale = MIN( (float)GetScreenWidth() / gameWidth, (float)GetScreenHeight() / gameHeight);
    //
    // input handling
    // movement
    Vector2 topLeftPoint = GetScreenToWorld2D( (Vector2){ 0, 0 }, camera);
    // int casting all coordinates and +- 1 to perfectly bound camera
    // if( CheckForFractionPortion(topLeftPoint.x) )
    //   topLeftPoint.x = DeleteFractionPortion(topLeftPoint.x) - 1;
    // if( CheckForFractionPortion(topLeftPoint.y) )
    //   topLeftPoint.y = DeleteFractionPortion(topLeftPoint.y) - 1;
    printf( "TOP LEFT X: %f, Y: %f\n", topLeftPoint.x, topLeftPoint.y );
    Vector2 bottomRightPoint = GetScreenToWorld2D( (Vector2){ gameWidth, gameHeight }, camera);
    // if( CheckForFractionPortion(bottomRightPoint.x) )
    //   bottomRightPoint.x = DeleteFractionPortion(bottomRightPoint.x) + 1;
    // if( CheckForFractionPortion(bottomRightPoint.y) )
    //   bottomRightPoint.y = DeleteFractionPortion(bottomRightPoint.y) + 1;
    printf( "BOTTOM RIGHT X: %f, Y: %f\n", bottomRightPoint.x, bottomRightPoint.y );
    Vector2 centerPoint = GetScreenToWorld2D( (Vector2){ windowWidth / 2.0f, windowHeight / 2.0f }, camera);
    printf( "Center Point X: %f, Y: %f\n", centerPoint.x, centerPoint.y );
    printf( "Map Dim. X: %f, Y: %f, W: %f, H: %f\n", mapRect.x, mapRect.y, mapRect.width, mapRect.height );

    if( topLeftPoint.x > mapRect.x )
    {
      if( IsKeyDown(KEY_LEFT) )
        camera.target.x -= (int)(cameraSpd * GetFrameTime() );
    }
    if( bottomRightPoint.x < mapRect.width )
    {
      if( IsKeyDown(KEY_RIGHT) )
        camera.target.x += (int)(cameraSpd * GetFrameTime() );
    }
    if( topLeftPoint.y > mapRect.y )
    {
      if( IsKeyDown(KEY_UP) )
        camera.target.y -= (int)(cameraSpd * GetFrameTime() );
    }
    if( bottomRightPoint.y < mapRect.height )
    {
      if( IsKeyDown(KEY_DOWN) )
        camera.target.y += (int)(cameraSpd * GetFrameTime() );
    }
    // reset
    if( IsKeyPressed(KEY_R) )
      camera.target = (Vector2){ (float)(map->tile_width * map->width) / 2, (float)(map->tile_height * map->height) / 2};
    // render tilemap to render texture
    BeginTextureMode(targetTexture);
      BeginMode2D(camera);
        RenderMap(map);
        DrawPixelV(topLeftPoint, RED);
        DrawPixelV(bottomRightPoint, RED);
      EndMode2D();
    EndTextureMode();
    // draw
    BeginDrawing();
      ClearBackground(BLACK);
      DrawTexturePro(targetTexture.texture, (Rectangle){0, 0, targetTexture.texture.width, -targetTexture.texture.height},
        (Rectangle){ (GetScreenWidth() - ( (float)gameWidth* scale) ) * 0.5f, (GetScreenHeight() - ( (float)gameHeight * scale) ) * 0.5f, (float)gameWidth * scale, (float)gameHeight * scale}, 
        (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
  }
  tmx_map_free(map);
  CloseWindow();

  return 0;
}

void* RaylibTexLoad( const char *path )
{
  Texture2D *texture = malloc( sizeof(Texture2D) );
  *texture = LoadTexture(path);
  return texture;
}
void RaylibTexFree( void *texture )
{
  UnloadTexture( *( (Texture2D*)&texture) );
  free(texture);
}

int DeleteFractionPortion( float number )
{
  float fractionChecker = number - (int)number;
  int fractionPortionDeleted = (int)number;

  if( fractionChecker > 0 && fractionChecker < 1 )
    fractionPortionDeleted = (int)number;
  else if( fractionChecker < 0 && fractionChecker > -1 )
    fractionPortionDeleted = (int)number;

  return fractionPortionDeleted;
}
bool CheckForFractionPortion( float number )
{
  float fractionChecker = number - (int)number;

  if( fractionChecker > 0 && fractionChecker < 1 )
    return true;
  else if( fractionChecker < 0 && fractionChecker > -1 )
    return true;

  return false;
}
