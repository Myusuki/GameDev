#include <raylib.h>

// global variables
#define screenWidth 800
#define screenHeight 450

int main(void)
{
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(screenWidth, screenHeight, "Gamepad Input Handling");
  SetTargetFPS(60);

  while(!WindowShouldClose() )
  {
    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      for( int gamePads = 0, textPosY = 5; gamePads < 4; gamePads++ ) // max 4 gamepads
      {
        if( IsGamepadAvailable(gamePads) )
        {
          DrawText(TextFormat("Gamepad Name: %s", GetGamepadName(gamePads) ), 10, textPosY, 10, BLACK);
          textPosY += 11;
          DrawText(TextFormat("\tAxis Count: %d", GetGamepadAxisCount(gamePads) ), 10, textPosY, 10, BLACK);
          textPosY += 11;

          for( int axes = 0; axes < GetGamepadAxisCount(gamePads); axes++ )
          {
            DrawText(TextFormat("\tAxis %d: %f", axes, GetGamepadAxisMovement(gamePads, axes) ), 10, textPosY, 10, BLACK);
            textPosY += 11;
          }
          for( int buttons = 0; buttons < 32; buttons++ )
          {
            DrawText(TextFormat("\tButton %d: %d", buttons, IsGamepadButtonDown(gamePads, buttons) ), 10, textPosY, 10, BLACK);
            textPosY += 11;
          }
        }
      }
      DrawFPS(GetScreenWidth() - 100, 100);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
