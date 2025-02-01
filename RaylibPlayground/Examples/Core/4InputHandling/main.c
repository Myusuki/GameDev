#include <raylib.h>

// global variables
#define screenWidth 800
#define screenHeight 450

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Input Handling");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  // position of Ball to be toyed with
  Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };
  // color of ball
  Color ballColor = DARKBLUE;
  // y-axis scroll speed of ball
  int scrollSpeed = 4;

  // game loop
  while( !WindowShouldClose() )
  {
    // logic update
    // mouse position
    Vector2 mousePosition = GetMousePosition();
    // arrow keys for movement
    if( IsKeyDown(KEY_RIGHT) )
      ballPosition.x += 1.0f;
    if( IsKeyDown(KEY_LEFT) )
      ballPosition.x -= 1.0f;
    if( IsKeyDown(KEY_UP) )
      ballPosition.y -= 1.0f;
    if( IsKeyDown(KEY_DOWN) )
      ballPosition.y += 1.0f;
    // mouse keys for changing color
    if( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) )
      ballColor = MAROON;
    else if( IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) )
      ballColor = LIME;
    else if( IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
      ballColor = DARKBLUE;
    ballPosition.y -= GetMouseWheelMove() * scrollSpeed;

    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      DrawCircleV(ballPosition, 50, ballColor);

      DrawText("Move the ball with arrow keys", 10, 10, 20, DARKGRAY);
      DrawText(TextFormat("%d, %d", (int)mousePosition.x, (int)mousePosition.y), (screenWidth - MeasureText(TextFormat("%d, %d", (int)mousePosition.x, (int)mousePosition.y), 50)) / 2, screenHeight / 4, 50, DARKGRAY);
      DrawText(TextFormat("Box Position Y: %03i", (int) ballPosition.y), 10, 40, 20, DARKGRAY);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
