#include <raylib.h>

// global variables
#define screenWidth 800
#define screenHeight 450

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Random Value Generator");
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);

  // variables
  int randomValue = GetRandomValue(-8, 5);
  unsigned int framesCounter = 0;

  while( !WindowShouldClose() )
  {
    // update
    framesCounter++;
    // logic
    if( framesCounter % 120 == 0 )
    {
      randomValue = GetRandomValue(-8, 5);
      framesCounter = 0;
    }
    // draw 
    BeginDrawing();
      ClearBackground(RAYWHITE);

      DrawText("Every 2 seconds a new random value is generated", (screenWidth - MeasureText("Every 2 seconds a new random value is generated", 20) ) / 2, screenHeight / 3, 20, MAROON);
      DrawText(TextFormat("%d", randomValue), (screenWidth - MeasureText(TextFormat("%d", randomValue), 80) ) / 2, screenHeight / 2, 80, GRAY);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
