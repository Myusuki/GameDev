#include <raylib.h>
#include <stdbool.h>

// global variables
#define screenWidth 800
#define screenHeight 450

int main(void)
{
  // init window
  InitWindow(screenWidth, screenHeight, "Window Should Close");
  SetTargetFPS(60); // set target fps to 60

  // turn off escape key for exiting game
  SetExitKey(KEY_NULL);

  bool exitWindowRequested = false;
  bool exitWindow = false;

  // game loop 
  while( !exitWindow )
  {
    // update
    // detect if X button or escape key is pressed
    if( WindowShouldClose() )
      exitWindowRequested = true;

    if( exitWindowRequested )
    {
      // ask to close program
      if( IsKeyPressed(KEY_Y) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) )
        exitWindow = true;
      else if( IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE) )
        exitWindowRequested = false;
    }

    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      if( exitWindowRequested )
      {
        DrawRectangle(0, 100, screenWidth, 200, BLACK);
        DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30, WHITE);
      }
      else
        DrawText("Try to close the window to get confirmation message!", 120, 200, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
