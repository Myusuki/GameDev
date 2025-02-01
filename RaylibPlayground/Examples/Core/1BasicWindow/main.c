#include <raylib.h> // header needed to use raylib

// global variables
#define screenWidth 800
#define screenHeight 450

int main(void)
{
  // init window
  InitWindow(screenWidth, screenHeight, "Basic Window");
  SetTargetFPS(60); // set target fps to 60 
  
  // main game loop
  while( !WindowShouldClose() ) // detects if alt+f4, window close button, or escape is pressed
  {
    // update
    //  logic and input handling happens here
    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE); // clear the screen and make background white like priming with white primer
      DrawText("Congrats! You created your first window!", 190, 200, 20, GRAY); // draw text no duh
    EndDrawing();
  }

  // de-initialization or quitting out of everything
  CloseWindow();

  return 0;
}
