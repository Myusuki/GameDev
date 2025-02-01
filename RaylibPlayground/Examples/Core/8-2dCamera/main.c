#include "raylib-5.5/src/raylib.h"

// global variables
// window dimensions
#define windowWidth 800
#define windowHeight 450
// max buildings
#define maxBuildings 100


int main(void)
{
  InitWindow(windowWidth, windowHeight, "2d Camera");
  SetTargetFPS(60);

  // create player
  Rectangle player = { 400, 280, 40, 40 };

  // create buidlings
  Rectangle buildings[maxBuildings] = { 0 };
  Color buildingColors[maxBuildings] = { 0 };
  int buildingSpacing = 0;
  for( int building = 0; building < maxBuildings; building++ )
  {
    buildings[building].width = (float)GetRandomValue(50, 200);
    buildings[building].height = (float)GetRandomValue(100, 800);
    buildings[building].y = windowHeight - 130.0f - buildings[building].height;
    buildings[building].x = -6000.0f + buildingSpacing;
    buildingColors[building] = (Color){ GetRandomValue(200, 240), GetRandomValue(200, 240), GetRandomValue(200, 250), 255 };

    buildingSpacing += (int)buildings[building].width;
  }
  
  // create Camera2D
  Camera2D camera = { 0 };
  camera.target = (Vector2){ player.x + (player.width / 2), player.y + (player.height / 2) };
  camera.offset = (Vector2){ windowWidth / 2.0f, windowHeight / 2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  while( !WindowShouldClose() )
  {
    // input handling
    // player movement
    if( IsKeyDown(KEY_RIGHT) )
      player.x += 2;
    else if( IsKeyDown(KEY_LEFT) )
      player.x -= 2;
    if( (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ) && IsKeyDown(KEY_RIGHT) )
      player.x += 10;
    else if( (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ) && IsKeyDown(KEY_LEFT) )
      player.x -= 10;
    // camera rotation controls
    if( IsKeyDown(KEY_A) )
      camera.rotation--;
    else if( IsKeyDown(KEY_S) )
      camera.rotation++;
    // camera zoom controls
    camera.zoom += (float)GetMouseWheelMove() * 0.05f;
    // camera reset
    if( IsKeyPressed(KEY_R) )
    {
      camera.zoom = 1.0f;
      camera.rotation = 0.0f;
    }

    // logic
    // camera target players
    camera.target = (Vector2){ player.x + 20, player.y + 20 };
    // limit camera rotation to 80 controls (-40 to 40)
    if( camera.rotation > 40 )
      camera.rotation = 40;
    else if( camera.rotation < -40 )
      camera.rotation = -40;
    // limit camera zoom 
    if( camera.zoom > 3.0f )
      camera.zoom = 3.0f;
    else if( camera.zoom < 0.1f )
      camera.zoom = 0.1f;

    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      // 2d camera mode
      BeginMode2D(camera);
        DrawRectangle(-6000, 320, 13000, 8000, DARKGRAY);

        for( int building = 0; building < maxBuildings; building++ )
          DrawRectangleRec(buildings[building], buildingColors[building] );

        DrawRectangleRec(player, RED);

        DrawLine((int)camera.target.x, -windowHeight*10, (int)camera.target.x, windowHeight*10, GREEN);
        DrawLine(-windowWidth*10, (int)camera.target.y, windowHeight*10, (int)camera.target.y, GREEN);
      EndMode2D();

      DrawText("Screen Area", 640, 10, 20, RED);

      DrawRectangle(0, 0, windowWidth, 5, RED);
      DrawRectangle(0, 5, 5, windowHeight - 10, RED);
      DrawRectangle(windowWidth - 5, 5, 5, windowHeight - 10, RED);
      DrawRectangle(0, windowHeight - 5, windowWidth, 5, RED);

      DrawRectangle(10, 10, 250, 113, Fade(SKYBLUE, 0.5f) );
      DrawRectangleLines(10, 10, 250, 113, BLUE);

      DrawText("Free 2d camera controls:", 20, 20, 10, BLACK);
      DrawText("- Right/Left to move Offset", 40, 40, 10, DARKGRAY);
      DrawText("- Mouse Wheel to Zoom in-out", 40, 60, 10, DARKGRAY);
      DrawText("- A/S to Rotate", 40, 80, 10, DARKGRAY);
      DrawText("- R to reset Zoom and Rotation", 40, 100, 10, DARKGRAY);

    EndDrawing();
  }
  CloseWindow();

  return 0;
}
