#include <raylib.h>

// global variables
#define screenWidth 800
#define screenHeight 450

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Scissor Test");
  SetTargetFPS(60);

  // viewing rectangle
  Rectangle scissorArea = { 0, 0, 300, 300 };
  bool scissorMode = true;

  while( !WindowShouldClose() )
  {
    // input handling
    if( IsKeyPressed(KEY_S) )
      scissorMode = !scissorMode;
    // logic
    // center the scissor area around the mouse pointer
    scissorArea.x = GetMouseX() - scissorArea.width / 2;
    scissorArea.y = GetMouseY() - scissorArea.height / 2;
    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      if(scissorMode)
        BeginScissorMode( (int)scissorArea.x, (int)scissorArea.y, (int)scissorArea.width, (int)scissorArea.height);

      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);
      DrawText("Move the mouse around to reveal this text", 190, 200, 20, LIGHTGRAY);

      if(scissorMode)
        EndScissorMode();
      DrawRectangleLinesEx(scissorArea, 1, BLACK);
      DrawText("Press S to toggle scissor test", 10, 10, 20, BLACK);

    EndDrawing();
  }
  CloseWindow();

  return 0;
} 
