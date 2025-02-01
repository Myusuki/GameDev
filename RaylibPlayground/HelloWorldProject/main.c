#include <raylib.h>

typedef enum GameScreen{ TITLE, GAMEPLAY, ENDING } GameScreen;

typedef struct Player
{
  Vector2 position;
  Vector2 speed;
  Vector2 size;
  Rectangle bounds;
} Player;

typedef struct Wall
{
  Vector2 position;
  Vector2 size;
  Rectangle bounds;
} Wall;

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Learning Project");

  GameScreen screen = TITLE;
  int frameCounter = 0;
  bool gamePaused = false;

  Vector2 center = { (float)(GetScreenWidth() / 2), (float)(GetScreenHeight() / 2) };

  Player player = {0};
  Wall wall = {0};

  player.size = (Vector2){ 50, 50 };
  player.position = (Vector2){ (center.x - player.size.x), (center.y - player.size.y) };
  player.speed = (Vector2){ 8.0f, 16.0f };

  wall.size = (Vector2){ 100, 50 };
  wall.position = (Vector2){ (center.x - wall.size.x), (center.y - wall.size.y + 200) };

  SetTargetFPS(60);

  while( !WindowShouldClose() )
  {
    switch( screen )
    {
      case TITLE:
      {
        frameCounter++;
        if( IsKeyPressed(KEY_ENTER) )
        {
          screen = GAMEPLAY;
        }
      } break;
      case GAMEPLAY:
      {
        frameCounter++;

        if( !gamePaused )
        {
          player.position.y += player.speed.y;
          if( IsKeyDown(KEY_LEFT) )
            player.position.x -= player.speed.x;
          else if( IsKeyDown(KEY_RIGHT) )
            player.position.x += player.speed.x;
          if( IsKeyDown(KEY_UP) )
            player.position.y -= player.speed.y * 2;

          if( player.position.x <= 0 || (player.position.x + player.size.x) >= screenWidth )
            player.position.x = center.x;
          if( player.position.y <= 0 || (player.position.y + player.size.y) >= screenHeight )
            player.position.y = center.y;
        }

        if( IsKeyPressed(KEY_SPACE) )
        {
          screen = ENDING;
        }

      } break;
      case ENDING:
      {
        frameCounter++;
        if( IsKeyPressed(KEY_ENTER) )
        {
          screen = TITLE;
        }
      } break;
    }
    
    BeginDrawing();
    ClearBackground(WHITE);

    switch( screen )
    {
      case TITLE:
      {
        DrawText("Hello World!", GetScreenWidth() / 2 - MeasureText("Hello World!", 40) / 2, 20, 40, BLACK);

        if( (frameCounter/30) % 2 != 0 )
          DrawText("Press [ENTER] To Play!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] To Play!", 30) / 2, GetScreenHeight() / 2, 30, BLACK);
      } break;
      case GAMEPLAY:
      {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, WHITE);
      } break;
      case ENDING:
      {
        DrawText("Press [ENTER] to Restart!", GetScreenWidth() / 2 - MeasureText("Press [ENTER] to Restart!", 20) / 2, GetScreenHeight() / 2 - 60, 20, BLACK);
      } break;
    }

    EndDrawing();
  }
  CloseWindow();

  return 0;
}
