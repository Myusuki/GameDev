#include <raylib.h>

// global variables
#define screenWidth 800
#define screenHeight 450

int main( int argc, char *argv[] )
{
  InitWindow(screenWidth, screenHeight, "Input Test");
  SetTargetFPS(60);

  SetExitKey(KEY_NULL);

  int boxPositionY = screenWidth / 2 - 40;
  int scrollSpeed = 4;

  while( !WindowShouldClose() )
  {
    boxPositionY -= (int)( GetMouseWheelMove() * scrollSpeed );
    Vector2 mousePos = GetMousePosition();
    int mousePosX = mousePos.x;
    int mousePosY = mousePos.y;

    BeginDrawing();
      ClearBackground(RAYWHITE);

      DrawRectangle(screenWidth / 2 - 40, boxPositionY, 80, 80, MAROON);
      DrawText("Use mouse wheel to move cube up and down!", 10, 10, 20, GRAY);
      DrawText(TextFormat("Mouse Position: %4i, %4i", mousePosX, mousePosY), 10, 40, 20, LIGHTGRAY);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
