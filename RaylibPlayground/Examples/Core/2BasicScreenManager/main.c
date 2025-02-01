#include <raylib.h> // raylib header

// global variables
#define screenWidth 800
#define screenHeight 450

// types and struct definitions
typedef enum GameScreen{ LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen; // app screens

int main(void)
{
  // init window
  InitWindow(screenWidth, screenHeight, "Basic Screen Manager");
  SetTargetFPS(60); // set target fps to 60

  // setting variables
  GameScreen currentScreen = LOGO;
  int framesCounter = 0; // count frames

  // main game loop
  while( !WindowShouldClose() ) // close window when alt+f4, close button, or escape is pressed
  {
    // update
    // logic and input handling
    switch( currentScreen )
    {
      case LOGO:
        framesCounter++; // count frames
        
        // wait for 2 seconds (120 frames cause 60 frames = 1 second) before jumping to title screen
        if( framesCounter > 120 )
          currentScreen = TITLE;
        break;
      case TITLE:
        // detect for enter key, gesture tap, or mouse click 
        if( IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) )
          currentScreen = GAMEPLAY;
        break;
      case GAMEPLAY:
        // detect for enter key, gesture tap, or mouse click 
        if( IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) )
          currentScreen = ENDING;
        break;
      case ENDING:
        // detect for enter key, gesture tap, or mouse click 
        if( IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) )
          currentScreen = TITLE;
        break;
      default:
        break;
    }

    // draw
    BeginDrawing();
      ClearBackground(RAYWHITE);

      switch( currentScreen )
      {
        case LOGO:
          // draw logo screen
          DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
          DrawText("WAIT FOR 2 SECONDS...", 290, 220, 20, GRAY);
          break;
        case TITLE:
          // draw title screen
          DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
          DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
          DrawText("PRESS ENTER or TAP to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
          break;
        case GAMEPLAY:
          // draw gameplay screen
          DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
          DrawText("TITLE SCREEN", 20, 20, 40, MAROON);
          DrawText("PRESS ENTER or TAP to JUMP to ENDING SCREEN", 120, 220, 20, MAROON);
          break;
        case ENDING:
          // draw ending sreen
          DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
          DrawText("ENDING SCREEN", 20, 20, 40, DARKBLUE);
          DrawText("PRESS ENTER or TAP to RETURN to TITLE SCREEN", 120, 220, 20, DARKBLUE);
          break;
        default:
          break;
      }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
