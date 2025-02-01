#include "raylib.h"

#define PLAYER_LIVES 			 5
#define BRICKS_LINES 			 5
#define BRICKS_PER_LINE 	20
#define BRICKS_POSITION_Y 50

typedef struct Player 
{
	Vector2 position;
	Vector2 speed;
	Vector2 size;
	Rectangle bounds;
	int lives;
} Player;

typedef struct Ball
{
	Vector2 position;
	Vector2 speed;
	float radius;
	bool active;
} Ball;

typedef struct Brick
{
	Vector2 position;
	Vector2 size;
	Rectangle bounds;
	int resistance;
	bool active;
} Brick;

// game screens to cycle thru
typedef enum Gamescreen{ LOGO, TITLE, GAMEPLAY, ENDING } Gamescreen;

int main(void)
{
	// window dimensions
	const int screenWidth = 800;
	const int screenHeight = 450;
  
  // init window
  InitWindow(screenWidth, screenHeight, "Project: Breakout");
  
	// initial game screen
	Gamescreen screen = LOGO;

	int framesCounter = 0;
	int gamesResult = -1; // 0 = lose, 1 = win, -1 = nothing
	bool gamePaused = false; // game pause toggle

	Player player = { 0 }; // initialize player and every member of the struct to 0 
	Ball ball = { 0 }; // same as player, intialize it and every member to 0 
	Brick brick[BRICKS_LINES][BRICKS_PER_LINE] = { 0 };

	// init player 
	player.position = (Vector2){ (float)screenWidth / 2, (float)screenHeight * 7/8 };
	player.speed = (Vector2){ 8.0f, 0.0f };
	player.size = (Vector2){ 100, 24 };
	player.lives = PLAYER_LIVES;

	// init ball
	ball.radius = 10.0f;
	ball.active = false;
	ball.position = (Vector2){ (player.position.x + player.size.x / 2), (player.position.y - ball.radius * 2) };
	ball.speed = (Vector2){ 4.0f, 4.0f };

	// init bricks 
	for( int row = 0; row < BRICKS_LINES; row++ )
	{
		for( int column = 0; column < BRICKS_PER_LINE; column++ )
		{
			brick[row][column].size = (Vector2){ (float)screenWidth / BRICKS_PER_LINE, 20 };
			brick[row][column].position = (Vector2){ column * brick[row][column].size.x, row * brick[row][column].size.y + BRICKS_POSITION_Y };
			brick[row][column].bounds = (Rectangle){ brick[row][column].position.x, brick[row][column].position.y, 
																								brick[row][column].size.x, brick[row][column].size.y };
			brick[row][column].active = true;
		}
	}

	// set target fps
	SetTargetFPS(60);

	// main game loop
	while( !WindowShouldClose() )
	{
		// logic
		switch( screen )
		{
			case LOGO:
			{
				framesCounter++;

				// change screen after 3 seconds; 1 sec = 60 frames 
				if( framesCounter > 180 )
				{
          framesCounter = 0;
          screen = TITLE;
				}
			} break;
			case TITLE:
			{
				framesCounter++;

				if( IsKeyPressed(KEY_ENTER) )
					screen = GAMEPLAY;
			} break;
			case GAMEPLAY:
			{
				if( !gamePaused )
				{
					// gameplay logic
				}
			} break;
			case ENDING:
			{
				framesCounter++;

				if( IsKeyPressed(KEY_ENTER) )
					screen = TITLE;	
			} break;
			default:
				break;
		}

		BeginDrawing();
		
		ClearBackground(RAYWHITE);

		switch( screen )
		{
			case LOGO:
			{
				DrawText("Logo Screen", (screenWidth / 2 - MeasureText("Logo Screen", 40) / 2), (screenHeight / 2 - 10), 40, LIGHTGRAY);
			} break;
			case TITLE:
			{
				DrawText("Title Screen", 20, 20, 40, DARKGREEN);

				if( (framesCounter / 30) % 2 == 0 )
					DrawText("Press [ENTER] to Start!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to Start!", 20) / 2, GetScreenHeight() / 2 + 60, 20, DARKGRAY);
			} break;
			case GAMEPLAY:
			{
				DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLACK);
				DrawCircle(ball.position.x, ball.position.y, ball.radius, MAROON);

				for( int rows = 0; rows < BRICKS_LINES; rows++ )
				{
					for( int columns = 0; columns < BRICKS_PER_LINE; columns++ )
					{
							if( brick[rows][columns].active )
							{
								if( (rows + columns) % 2 == 0 )
									DrawRectangle(brick[rows][columns].position.x, brick[rows][columns].position.y, brick[rows][columns].size.x, brick[rows][columns].size.y, GRAY);
								else
									DrawRectangle(brick[rows][columns].position.x, brick[rows][columns].position.y, brick[rows][columns].size.x, brick[rows][columns].size.y, DARKGRAY);
							}
						}
				}
        
        for( int lives = 0; lives < player.lives; lives++ )
          DrawRectangle(20 + 40 * lives, screenHeight - 30, 35, 10, LIGHTGRAY);

        if( gamePaused )
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 + 60, 40, GRAY);
			} break;
      case ENDING:
      {
        DrawText("End Screen", 20, 20, 40, DARKBLUE);

        if( (framesCounter / 30 ) % 2 == 0 )
            DrawText("Press [ENTER] to play again!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to play again!", 20) / 2, GetScreenHeight() / 2 + 80, 20, GRAY);
      } break;
      default:
        break;
		}
		EndDrawing();
	}

  CloseWindow();
	return 0;
}
