// a little something for my 4th anniversary and for my gf
// will be improved down the line
#include <raylib.h>
#include <stdbool.h>

// global variables
#define screenWidth 1920
#define screenHeight 1080

// game screen enums
typedef enum GameScreen{ MENU = 0, GAMEPLAY } GameScreen;

int main(void)
{
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(screenWidth, screenHeight, "Simple Platformer");
  SetTargetFPS(60);

  GameScreen currentScreen = MENU;
  bool showText = false;
  int framesCounter = 0;

  while( !WindowShouldClose() || IsKeyPressed(KEY_ESCAPE) )
  {
    switch( currentScreen )
    {
      case MENU:
        framesCounter++;

        if( IsKeyPressed(KEY_ENTER) )
        {
          showText = true;
          currentScreen = GAMEPLAY;
        }
        break;
      case GAMEPLAY:
        framesCounter++;
        break;
      default:
        break;
    }

    BeginDrawing();
      ClearBackground(WHITE);
      switch( currentScreen )
      {
        case MENU:
          if( (framesCounter / 30) % 2 == 0 )
            DrawText("PRESS ENTER", (screenWidth - MeasureText("PRESS ENTER", 50) ) / 2, (screenHeight / 2), 50, BLACK);
          break;
        case GAMEPLAY:
          if( showText )
            DrawText("Happy ANNIVERSARY!!!", (screenWidth - MeasureText("Happy ANNIVERSARY!", 50) ) / 2 , (screenHeight / 2), 50, DARKPURPLE);
          break;
        default:
          break;
      }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
