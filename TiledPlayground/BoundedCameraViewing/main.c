#include <stdio.h>
#include <tmx.h>
#include <raylib.h>
#include "include/mapRender.h"

// modular funcs
bool CheckForFractionPortion( float number );
int DeleteFractionPortion( float number );

int main(int argc, char **argv)
{
	InitWindow(windowWidth, windowHeight, "raylib example");
	if (!IsWindowReady()) {
		fputs("Cannot create a window", stderr);
		return 1;
	}

	SetTargetFPS(30);
  SetExitKey(KEY_NULL);

	/* Set the callback globs in the main function */
	tmx_img_load_func = RaylibTexLoad;
	tmx_img_free_func = RaylibTexFree;

	tmx_map *map = tmx_load( argv[1] );
	if (!map) {
		tmx_perror("Cannot load map");
		return 1;
	}
  Rectangle mapRect = {0, 0, (map->tile_width * map->width), (map->tile_height * map->height) };

  Camera2D camera = { .target = (Vector2){ ( (float)(map->tile_width * map->width) / 2), ( (float)(map->tile_height * map->height) / 2) },
                      .offset = (Vector2){ ( (float)windowWidth / 2), ( (float)windowHeight / 2) },
                      .zoom = 5.0 };
  int cameraSpd = 128;

	while (!WindowShouldClose()) {
    // input handling
    // movement
    Vector2 topLeftPoint = GetScreenToWorld2D( (Vector2){ 0, 0 }, camera);
    // int casting all coordinates and +- 1 to perfectly bound camera
    if( CheckForFractionPortion(topLeftPoint.x) )
      topLeftPoint.x = DeleteFractionPortion(topLeftPoint.x) - 1;
    if( CheckForFractionPortion(topLeftPoint.y) )
      topLeftPoint.y = DeleteFractionPortion(topLeftPoint.y) - 1;
    printf( "TOP LEFT X: %f, Y: %f\n", topLeftPoint.x, topLeftPoint.y );
    Vector2 bottomRightPoint = GetScreenToWorld2D( (Vector2){ windowWidth, windowHeight }, camera);
    if( CheckForFractionPortion(bottomRightPoint.x) )
      bottomRightPoint.x = DeleteFractionPortion(bottomRightPoint.x) + 1;
    if( CheckForFractionPortion(bottomRightPoint.y) )
      bottomRightPoint.y = DeleteFractionPortion(bottomRightPoint.y) + 1;
    printf( "BOTTOM RIGHT X: %f, Y: %f\n", bottomRightPoint.x, bottomRightPoint.y );
    Vector2 centerPoint = GetScreenToWorld2D( (Vector2){ windowWidth / 2.0f, windowHeight / 2.0f }, camera);
    printf( "Center Point\nX: %f, Y: %f\n", centerPoint.x, centerPoint.y );
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

		BeginDrawing();
      BeginMode2D(camera);
        RenderMap(map);
      EndMode2D();
		EndDrawing();
	}

	tmx_map_free(map);
	CloseWindow();

	return 0;
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
