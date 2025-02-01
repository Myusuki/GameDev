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

  // Load textures
  Texture2D texLogo = LoadTexture("resources/raylib_logo.png");
  Texture2D texBall = LoadTexture("resources/ball.png");
  Texture2D texPaddle = LoadTexture("resources/paddle.png");
  Texture2D texBrick = LoadTexture("resources/brick.png");

  // Load fonts
  Font font = LoadFont("resources/setback.png");

	// initial game screen
	Gamescreen screen = LOGO;

	int framesCounter = 0;
	int gamesResult = -1; // 0 = lose, 1 = win, -1 = nothing
	bool gamePaused = false; // game pause toggle

	Player player = { 0 }; // initialize player and every member of the struct to 0 
	Ball ball = { 0 }; // same as player, intialize it and every member to 0 
	Brick brick[BRICKS_LINES][BRICKS_PER_LINE] = { 0 };

	// init player members
	player.position = (Vector2){ (float)screenWidth / 2, (float)screenHeight * 7/8 };
	player.speed = (Vector2){ 8.0f, 0.0f };
	player.size = (Vector2){ 100, 24 };
	player.lives = PLAYER_LIVES;

	// init ball members
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
        if( IsKeyPressed('P') )
          gamePaused = !gamePaused;

        // non-paused gameplay 
				if( !gamePaused )
				{
          // player movement - left/right
          if( IsKeyDown(KEY_LEFT) )
            player.position.x -= player.speed.x;
          if( IsKeyDown(KEY_RIGHT) )
            player.position.x += player.speed.x;
          
          // movement boundaries
          if( player.position.x <= 0 ) // player.position.x is the top left corner of the rectangle
            player.position.x = 0;
          if( (player.position.x + player.size.x) >= screenWidth )
            player.position.x = screenWidth - player.size.x;

          // struct Rectangle{ num1 = top-left corner x position, num2 = top-left corner y position, num3 = width, num4 = height }
          player.bounds = (Rectangle){ player.position.x, player.position.y, player.size.x, player.size.y };

          if( ball.active )
          {
            // ball movement logic
            ball.position.x += ball.speed.x;
            ball.position.y += ball.speed.y;
            
            // ball wall collision logic 
            // reflection logic: if ball hits a screen limit, speed will be deflected to the respective direction causing the ball to move in the opposite direction 
            if( (ball.position.x + ball.radius) >= screenWidth || (ball.position.x - ball.radius) <= 0 ) 
              ball.speed.x *= -1;
            if( (ball.position.y - ball.radius) <= 0 )
              ball.speed.y *= -1;
            
            // ball player collision logic
            if( CheckCollisionCircleRec(ball.position, ball.radius, player.bounds) )
            {
              ball.speed.y *= -1;
              ball.speed.x = (ball.position.x - (player.position.x + player.size.x / 2) ) / player.size.x * 5.0f;
            }

            // ball bricks collision logic
            for( int row = 0; row < BRICKS_LINES; row++ )
            {
              for( int column = 0; column < BRICKS_PER_LINE; column++ )
              {
                if( brick[row][column].active && (CheckCollisionCircleRec(ball.position, ball.radius, brick[row][column].bounds) ) )
                {
                  ball.speed.y *= -1;
                  brick[row][column].active = false;

                  break;
                }
              }
            }

            // Game ending logic
            if( (ball.position.y + ball.radius) >= screenHeight )
            {
              ball.position.x = player.position.x + player.size.x / 2;
              ball.position.y = player.position.y - ball.radius - 1.0f;
              ball.speed = (Vector2){ 0, 0 };
              ball.active = false;
              player.lives--;
            }
            if( player.lives == 0 )
            {
              gamesResult = 0;
              screen = ENDING;
              player.lives = 5;
              framesCounter = 0;
            }
            else
            {
              int dormantBricks = 0;
              for( int row = 0; row < BRICKS_LINES; row++ )
              {
                for( int column = 0; column < BRICKS_PER_LINE; column++ )
                {
                  if( brick[row][column].active == true )
                    gamesResult = -1;
                  else
                    dormantBricks++;
                }
              }
              if( dormantBricks == 100 )
              {
                gamesResult = 1;
                screen = ENDING;
              }
            }
          }
          // ball isn't active
          else
          {
            // reset ball position
            ball.position.x = player.position.x + player.size.x / 2;

            // game start logic
            if( IsKeyPressed(KEY_SPACE) )
            {
              ball.active = true;
              ball.speed = (Vector2){ 0, -5.0f };
            }
          }
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
        DrawTexture(texLogo, screenWidth / 2 - texLogo.width / 2, screenHeight / 2 - texLogo.height / 2, WHITE);
			} break;
			case TITLE:
			{
        DrawTextEx(font, "BLOCKS", (Vector2){ 100, 80 }, 160, 10, MAROON);

				if( (framesCounter / 30) % 2 == 0 )
					DrawText("Press [ENTER] to Start!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to Start!", 20) / 2, GetScreenHeight() / 2 + 60, 20, DARKGRAY);
			} break;
			case GAMEPLAY:
			{
        DrawTextureV(texPaddle, player.position, WHITE); // Draw player 
        DrawTexture(texBall, ball.position.x - ball.radius / 2, ball.position.y - ball.radius / 2, MAROON); // draw ball 
        
        for( int row = 0; row < BRICKS_LINES; row++ )
        {
          for( int column = 0; column < BRICKS_PER_LINE; column++ )
          {
            if( brick[row][column].active )
            {
              if( (row + column) % 2 == 0 )
                DrawTextureV(texBrick, brick[row][column].position, GRAY);
              else
                DrawTextureV(texBrick, brick[row][column].position, DARKGRAY);
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
        DrawTextEx(font, "GAME FINISHED", (Vector2){ 80, 100 }, 80, 6, MAROON);

        if( (framesCounter / 30 ) % 2 == 0 )
          DrawText("Press [ENTER] to play again!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to play again!", 20) / 2, GetScreenHeight() / 2 + 80, 20, GRAY);
      } break;
      default:
        break;
		}
		EndDrawing();
	}

  // unload all textures and free memory
  UnloadTexture(texBall);
  UnloadTexture(texPaddle);
  UnloadTexture(texBrick);

  UnloadFont(font);

  CloseWindow();
	return 0;
}
