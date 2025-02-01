#include "raylib.h"

int main(void)
{
	// enum of the type of screens the program will cycle thru
	typedef enum Gamescreen{ LOGO, TITLE, GAMEPLAY, ENDING } Gamescreen;

	// window dimensions
	const int screenWidth = 800;
	const int screenHeight = 450;

	// init window
	InitWindow(screenWidth, screenHeight, "Project: Breakout" );

	// initial screen 
	Gamescreen screen = LOGO;

	int framescounter = 0; 	 // frame counter
	int gamesResult = -1; 	 // 0 = lose, 1 = win, -1 = no result
	bool gamePaused = false; // pause toggle

	// set the target fps of game
	SetTargetFPS(60);

	// game loop - where the game actually happens
	while( !WindowShouldClose() )
	{
		// logic first 
		switch( screen )
		{
			case LOGO:
			{
				framescounter++;
				
				// 1 second = 60 frames, 180 = 3 seconds
				if( framescounter > 180 )
				{
					screen = TITLE;
					framescounter = 0;
				}
			} break;
			case TITLE:
			{
				framescounter++;

				if( IsKeyPressed(KEY_ENTER) )
					screen = GAMEPLAY;
			} break;
			case GAMEPLAY:
			{
				if( !gamePaused )
				{
					
				}

				if( IsKeyPressed(KEY_ENTER) )
					screen = ENDING;
			} break;
			case ENDING:
			{
				framescounter++;

				if( IsKeyPressed(KEY_ENTER) )
					screen = TITLE;
			} break;
			default:
				break;
		} 

		BeginDrawing();
		
		ClearBackground(RAYWHITE); // set background color, similar to primer

		switch( screen )
		{
			case LOGO:
			{
				DrawText("LOGO Screen", 20, 20, 40, LIGHTGRAY);
				DrawText("Wait for 3 seconds...", 290, 220, 20, GRAY);
			} break;
			case TITLE:
			{
				DrawRectangle(0, 0, screenWidth, screenHeight, GREEN);
				DrawText("Title Screen", 20, 20, 40, DARKGREEN);
				DrawText("Press Enter or Tap to Jump to Gameplay Screen", 120, 220, 20, DARKGREEN);
			} break;
			case GAMEPLAY:
			{
				DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
				DrawText("Gameplay Screen", 20, 20, 40, MAROON);
				DrawText("Press Enter or Tap to jump to Ending Screen", 130, 220, 20, MAROON);
			} break;
			case ENDING:
			{
				DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
				DrawText("Ending Screen", 20, 20, 40, DARKBLUE);
				DrawText("Press Enter or Tap to jump to return to Title Screeen", 120, 220, 20, DARKBLUE);
			} break;
			default:
				break;
		}

		EndDrawing();
	}

	CloseWindow(); // deinits window

	return 0;
}
