#include <raylib.h>
#include <stdbool.h>

#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

typedef struct Snake
{
  Vector2 position;
  Vector2 size;
  Vector2 speed;
  Color color;
} Snake;

typedef struct Food
{
  Vector2 position;
  Vector2 size;
  bool active;
  Color color;
} Food;

// global variables
static const int screenWidth = 800;
static const int screenHeight = 450;

static int frameCounter = 0;
static bool gameOver = false;
static bool pause = false;

static Food food = { 0 };
static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static bool allowMove = false;
static Vector2 offset = { 0 };
static int counterTail = 0;

// modular functions of game
static void InitGame(void);
static void UpdateGame(void); // update game 1 frame
static void DrawGame(void); // draw game 1 frame
static void UnloadGame(void);
static void UpdateDrawFrame(void);  // update and draw game 1 frame

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Project: Snake" );

  InitGame();

  SetTargetFPS(60);

  while( !WindowShouldClose() )
  {
    // update and draw
    UpdateDrawFrame();
  }

  UnloadGame();
  CloseWindow();

  return 0;
}

void InitGame(void)
{
  frameCounter = 0;
  gameOver = false;
  pause = false;

  counterTail = 1;
  allowMove = false;

  offset.x = screenWidth % SQUARE_SIZE;
  offset.y = screenHeight % SQUARE_SIZE;

  for( int length = 0; length < SNAKE_LENGTH; length++ )
  {
    snake[length].position = (Vector2){ offset.x / 2, offset.y / 2 };
    snake[length].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
    snake[length].speed = (Vector2){ SQUARE_SIZE, 0 };

    if( length == 0 )
      snake[length].color = DARKBLUE;
    else
      snake[length].color = BLUE;
  }
  for( int length = 0; length < SNAKE_LENGTH; length++ )
  {
    snakePosition[length] = (Vector2){ 0.0f, 0.0f };
  }
  
  food.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
  food.color = GREEN;
  food.active = false;
}

void UpdateGame(void)
{
  if( !gameOver )
  {
    if( IsKeyPressed('P') )
      pause = !pause;

    if( !pause )
    {
      // controls
      if( (IsKeyPressed(KEY_RIGHT) || IsKeyPressed('D') ) && (snake[0].speed.x == 0) && allowMove )
      {
        snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
        allowMove = false;
      }
      if( (IsKeyPressed(KEY_LEFT) || IsKeyPressed('A') ) && (snake[0].speed.x == 0) && allowMove )
      {
        snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
        allowMove = false;
      }
      if( (IsKeyPressed(KEY_UP) || IsKeyPressed('W') ) && (snake[0].speed.y == 0) && allowMove )
      {
        snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
        allowMove = false;
      }
      if( (IsKeyPressed(KEY_DOWN) || IsKeyPressed('S') ) && (snake[0].speed.y == 0) && allowMove )
      {
        snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
        allowMove = false;
      }
      
      // snake movement
      for( int counter = 0; counter < counterTail; counter++ )
        snakePosition[counter] = snake[counter].position;

      if( (frameCounter % 5) == 0 )
      {
        for( int counter = 0; counter < counterTail; counter++ )
        {
          if( counter == 0 )
          {
            snake[0].position.x += snake[0].speed.x;
            snake[0].position.y += snake[0].speed.y;
            allowMove = true;
          }
          else
            snake[counter].position = snakePosition[counter - 1];
        }
      }

      // wall behavior
      if( snake[0].position.x > (screenWidth - offset.x) ||
          snake[0].position.y > (screenHeight - offset.y) ||
          snake[0].position.x < 0 || snake[0].position.y < 0 )
      {
        gameOver = true;
      }
      
      // collision with self
      for( int counter = 1; counter < counterTail; counter++ )
      {
        if( (snake[0].position.x == snake[counter].position.x) && (snake[0].position.y == snake[counter].position.y) )
          gameOver = true;
      }

      // calculate food position
      if( !food.active )
      {
        food.active = true;
        food.position = (Vector2){ GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2 };

        for( int counter = 0; counter < counterTail; counter++ )
        {
          while( snake[counter].position.x == food.position.x && snake[counter].position.y == food.position.y )
          {
            food.position = (Vector2){ GetRandomValue(0, (screenWidth / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.x / 2, GetRandomValue(0, (screenHeight / SQUARE_SIZE) - 1) * SQUARE_SIZE + offset.y / 2 };
            counter = 0;
          }
        }
      }
        
      // fruit collision
      if( (snake[0].position.x < (food.position.x + food.size.x) && (snake[0].position.x + snake[0].size.x) > food.position.x) &&
         snake[0].position.y < (food.position.y + food.size.y) && (snake[0].position.y + snake[0].size.y) > food.position.y )
      {
        snake[counterTail].position = snakePosition[counterTail - 1];
        counterTail++;
        food.active = false;
      }
      frameCounter++;
    }
  }
  else
  {
    if( IsKeyPressed(KEY_ENTER) )
    {
      InitGame();
      gameOver = false;
    }
  }
}

void DrawGame(void)
{
  BeginDrawing();
  ClearBackground(WHITE);
  
  if( !gameOver )
  {
    // draw grid lines
    for( int lines = 0; lines < screenWidth / SQUARE_SIZE; lines++ )
      DrawLineV( (Vector2){SQUARE_SIZE * lines + offset.x / 2, offset.y / 2}, (Vector2){ SQUARE_SIZE * lines + offset.x / 2, screenHeight - offset.y / 2}, LIGHTGRAY);
    for( int lines = 0; lines < screenHeight / SQUARE_SIZE; lines++ )
    {
      DrawLineV( (Vector2){offset.x / 2, SQUARE_SIZE * lines + offset.y / 2}, (Vector2){ screenWidth - offset.x / 2, SQUARE_SIZE * lines + offset.y / 2}, LIGHTGRAY);
    }

    // draw snake 
    for( int counter = 0; counter < counterTail; counter++ )
      DrawRectangle(snake[counter].position.x, snake[counter].position.y, snake[counter].size.x, snake[counter].size.y, snake[counter].color);

    DrawRectangleV(food.position, food.size, food.color);

    if( pause )
      DrawText("Game Paused", screenWidth / 2 - MeasureText("Game Paused", 40) / 2, screenHeight / 2 - 40 , 40, GRAY);
  }
  else
    DrawText("Press [Enter] to play again", GetScreenWidth() / 2 - MeasureText("Press [Enter] to play again", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);
  EndDrawing();
}

void UnloadGame(void)
{}

void UpdateDrawFrame(void)
{
  UpdateGame();
  DrawGame();
}
