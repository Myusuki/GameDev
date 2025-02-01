#include <raylib.h>
#include <math.h>

#define PLAYER_BASE_SIZE 20.0f
#define PLAYER_SPEED 6.0f
#define PLAYER_MAX_SHOTS 10

#define METEORS_SPEED 2
#define MAX_BIG_METEORS 4
#define MAX_MEDIUM_METEORS 8
#define MAX_SMALL_METEORS 16

// structures
typedef struct Player
{
  Vector2 position;
  Vector2 speed;
  float acceleration;
  float rotation;
  Vector3 collider;
  Color color;
} Player;

typedef struct Bullet
{
  Vector2 position;
  Vector2 speed;
  float radius;
  float rotation;
  int lifeSpawn;
  bool active;
  Color color;
} Bullet;

typedef struct Meteor
{
  Vector2 position;
  Vector2 speed;
  float radius;
  bool active;
  Color color;
} Meteor;

// global variables
static const int screenWidth = 800;
static const int screenHeight = 450;

static bool gameOver = false;
static bool pause = false;
static bool victory = false;

// used to make triangle for ship
static float shipHeight = 0.0f;

static Player player = { 0 };
static Bullet bullet[PLAYER_MAX_SHOTS] = { 0 };
static Meteor bigMeteor[MAX_BIG_METEORS] = { 0 };
static Meteor midMeteor[MAX_MEDIUM_METEORS] = { 0 };
static Meteor smallMeteor[MAX_SMALL_METEORS] = { 0 };

static int midMeteorCount = 0;
static int smallMeteorCount = 0;
static int destroyedMeteorCount = 0;

// modular functions
static void InitGame(void);
static void UpdateGame(void);
static void DrawGame(void);
static void UpdateDrawGame(void);
static void UnloadGame(void);

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Project: Asteroid" );

  InitGame();

  SetTargetFPS(60);

  while( !WindowShouldClose() )
  {
    UpdateDrawGame();
  }

  UnloadGame();
  CloseWindow();

  return 0;
}

void InitGame(void)
{
  int posX, posY;
  int velX, velY;
  bool correctRange = false;
  pause = false;
  victory = false;

  shipHeight = (PLAYER_BASE_SIZE / 2) / tanf(20 * DEG2RAD);

  // init player
  player.position = (Vector2){ screenWidth / 2, screenHeight / 2 - shipHeight / 2 };
  player.speed = (Vector2){ 0, 0 };
  player.acceleration = 0;
  player.rotation = 0;
  player.collider = (Vector3){ player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };
  player.color = RED;

  destroyedMeteorCount = 0;

  // init bullets
  for( int bullets = 0; bullets < PLAYER_MAX_SHOTS; bullets++ )
  {
    bullet[bullets].position = (Vector2){ 0, 0 };
    bullet[bullets].speed = (Vector2){ 0, 0 };
    bullet[bullets].radius = 2;
    bullet[bullets].active = false;
    bullet[bullets].lifeSpawn = 0;
    bullet[bullets].color = GRAY;
  }

  // init meteors
  // init big meteors
  for( int meteor = 0; meteor < MAX_BIG_METEORS; meteor++ )
  {
    // calculate big meteor position
    posX = GetRandomValue(0, screenWidth);

    while( !correctRange )
    {
      if( posX > screenWidth / 2 - 150 && posX < screenWidth / 2 + 150 )
        posX = GetRandomValue(0, screenWidth);
      else
        correctRange = true;
    }

    correctRange = false;

    posY = GetRandomValue(0, screenHeight);

    while( !correctRange )
    {
      if( posY > screenHeight / 2 - 150 && posY < screenHeight / 2 + 150 )
        posY = GetRandomValue(0, screenHeight);
      else
        correctRange = true;
    }

    bigMeteor[meteor].position = (Vector2){ posX, posY }; 

    correctRange = false;

    // calculate velocity
    velX = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
    velY = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);

    while( !correctRange )
    {
      if( velX == 0 && velY == 0 )
      {
        velX = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
        velY = GetRandomValue(-METEORS_SPEED, METEORS_SPEED);
      }
      else
        correctRange = true;
    }
    bigMeteor[meteor].speed = (Vector2){ velX, velY };
    bigMeteor[meteor].radius = 40;
    bigMeteor[meteor].active = true;
    bigMeteor[meteor].color = BLUE;
  }

  // init medium meteors
  for( int meteor = 0; meteor < MAX_MEDIUM_METEORS; meteor++ )
  {
    midMeteor[meteor].position = (Vector2){ -100, -100 };
    midMeteor[meteor].speed = (Vector2){ 0, 0 };
    midMeteor[meteor].radius = 20;
    midMeteor[meteor].active = false;
    midMeteor[meteor].color = BLUE;
  }

  // init small meteors
  for( int meteor = 0; meteor < MAX_SMALL_METEORS; meteor++ )
  {
    smallMeteor[meteor].position = (Vector2){ -100, -100 };
    smallMeteor[meteor].speed = (Vector2){ 0, 0 };
    smallMeteor[meteor].radius = 10;
    smallMeteor[meteor].active = false;
    smallMeteor[meteor].color = BLUE;
  }
  
  midMeteorCount = 0;
  smallMeteorCount = 0;
}

void UpdateGame(void)
{
  if( !gameOver )
  {
    if( IsKeyPressed('P') )
      pause = !pause;

    if( !pause )
    {
      // player logic: rotation
      if( IsKeyDown(KEY_LEFT) || IsKeyDown('A') )
        player.rotation -= 5;
      if( IsKeyDown(KEY_RIGHT) || IsKeyDown('D') )
        player.rotation += 5;

      // player logic: speed 
      player.speed.x = sin(player.rotation * DEG2RAD) * PLAYER_SPEED;
      player.speed.y = cos(player.rotation * DEG2RAD) * PLAYER_SPEED;

      // player logic: acceleration
      if( IsKeyDown(KEY_UP) || IsKeyDown('W') )
      {
        if( player.acceleration < 1 )
          player.acceleration += 0.04f;
      }
      else
      {
        if( player.acceleration > 0 )
          player.acceleration -= 0.02f;
        else if( player.acceleration < 0 )
          player.acceleration = 0;
      }
      if( IsKeyDown(KEY_DOWN) || IsKeyDown('S') )
      {
        if( player.acceleration > 0 )
          player.acceleration -= 0.04f;
        else if( player.acceleration < 0 )
          player.acceleration = 0;
      }

      // player logic: movement
      player.position.x += player.speed.x * player.acceleration;
      player.position.y -= player.speed.y * player.acceleration;

      // collision logic: player vs walls
      if( player.position.x > screenWidth + shipHeight )
        player.position.x = -(shipHeight);
      else if( player.position.x < -(shipHeight) )
        player.position.x = screenWidth + shipHeight;
      if( player.position.y > screenHeight + shipHeight )
        player.position.y = -(shipHeight);
      else if( player.position.y < -(shipHeight) )
        player.position.y = screenHeight + shipHeight;

      // player shooting logic
      if( IsKeyPressed(KEY_SPACE) )
      {
        for( int shots = 0; shots < PLAYER_MAX_SHOTS; shots++ )
        {
          if( !bullet[shots].active )
          {
            bullet[shots].position = (Vector2){ player.position.x + sin(player.rotation * DEG2RAD) * shipHeight, player.position.y - cos(player.rotation * DEG2RAD) * shipHeight };
            bullet[shots].active = true;
            bullet[shots].speed.x = 1.5 * sin( player.rotation * DEG2RAD ) * PLAYER_SPEED;
            bullet[shots].speed.y = 1.5 * cos( player.rotation * DEG2RAD ) * PLAYER_SPEED;
            bullet[shots].rotation = player.rotation;
            break;
          }
        }
      }

      // bullet duration timer
      for( int shots = 0; shots < PLAYER_MAX_SHOTS; shots++ )
      {
        if( bullet[shots].active )
          bullet[shots].lifeSpawn++;
      }
      
      // bullet logic
      for( int shots = 0; shots < PLAYER_MAX_SHOTS; shots++ )
      {
        if( bullet[shots].active )
        {
          // movement
          bullet[shots].position.x += bullet[shots].speed.x;
          bullet[shots].position.y -= bullet[shots].speed.y;

          // collision with walls
          if( bullet[shots].position.x > screenWidth + bullet[shots].radius )
          {
            bullet[shots].active = false;
            bullet[shots].lifeSpawn = 0;
          }
          else if( bullet[shots].position.x < 0 - bullet[shots].radius )
          {
            bullet[shots].active = false;
            bullet[shots].lifeSpawn = 0;
          }
          if( bullet[shots].position.y > screenHeight + bullet[shots].radius )
          {
            bullet[shots].active = false;
            bullet[shots].lifeSpawn = 0;
          }
          else if( bullet[shots].position.y < 0 - bullet[shots].radius )
          {
            bullet[shots].active = false;
            bullet[shots].lifeSpawn = 0;
          }

          // bullet fired duration
          if( bullet[shots].lifeSpawn >= 60 )
          {
            bullet[shots].position = (Vector2){ 0, 0 };
            bullet[shots].speed = (Vector2){ 0, 0 };
            bullet[shots].lifeSpawn = 0;
            bullet[shots].active = false;
          }
        }
      }

      // collision logic: player and meteors 
      player.collider = (Vector3){ player.position.x + sin(player.rotation * DEG2RAD) * shipHeight / 2.5f, player.position.y - cos(player.rotation * DEG2RAD) * shipHeight / 2.5f, 12 };
      
      for( int meteor = 0; meteor < MAX_BIG_METEORS; meteor++ )
      {
        if( CheckCollisionCircles( (Vector2){ player.collider.x, player.collider.y }, player.collider.z, bigMeteor[meteor].position, bigMeteor[meteor].radius) && bigMeteor[meteor].active ) 
          gameOver = true;
      }
      for( int meteor = 0; meteor < MAX_MEDIUM_METEORS; meteor++ )
      {
        if( CheckCollisionCircles( (Vector2){ player.collider.x, player.collider.y }, player.collider.z, midMeteor[meteor].position, midMeteor[meteor].radius) && midMeteor[meteor].active )
          gameOver = true;
      }
      for( int meteor = 0; meteor < MAX_SMALL_METEORS; meteor++ )
      {
        if( CheckCollisionCircles( (Vector2){ player.collider.x, player.collider.y }, player.collider.z, smallMeteor[meteor].position, smallMeteor[meteor].radius) && smallMeteor[meteor].active )
          gameOver = true;
      }
      
      // meteor logic
      for( int meteor = 0; meteor < MAX_BIG_METEORS; meteor++ )
      {
        if( bigMeteor[meteor].active )
        {
          // movement
          bigMeteor[meteor].position.x += bigMeteor[meteor].speed.x;
          bigMeteor[meteor].position.y += bigMeteor[meteor].speed.y;

          // collision with wall
          if( bigMeteor[meteor].position.x > screenWidth + bigMeteor[meteor].radius )
            bigMeteor[meteor].position.x = -(bigMeteor[meteor].radius);
          else if( bigMeteor[meteor].position.x < 0 - bigMeteor[meteor].radius )
            bigMeteor[meteor].position.x = screenWidth + bigMeteor[meteor].radius;
          if( bigMeteor[meteor].position.y > screenHeight + bigMeteor[meteor].radius )
            bigMeteor[meteor].position.y = -(bigMeteor[meteor].radius);
          else if( bigMeteor[meteor].position.y < 0 - bigMeteor[meteor].radius )
            bigMeteor[meteor].position.y = screenHeight + bigMeteor[meteor].radius;
        }
      }
      for( int meteor = 0; meteor < MAX_MEDIUM_METEORS; meteor++ )
      {
        if( midMeteor[meteor].active )
        {
          // movement 
          midMeteor[meteor].position.x += midMeteor[meteor].speed.x;
          midMeteor[meteor].position.y += midMeteor[meteor].speed.y;

          // collision with walls
          if( midMeteor[meteor].position.x > screenWidth + midMeteor[meteor].radius )
            midMeteor[meteor].position.x = -(midMeteor[meteor].radius);
          else if( midMeteor[meteor].position.x < 0 - midMeteor[meteor].radius )
            midMeteor[meteor].position.x = screenWidth + midMeteor[meteor].radius;
          if( midMeteor[meteor].position.y > screenHeight + midMeteor[meteor].radius )
            midMeteor[meteor].position.y = -(midMeteor[meteor].radius);
          else if( midMeteor[meteor].position.y < 0 - midMeteor[meteor].radius )
            midMeteor[meteor].position.y = screenHeight + midMeteor[meteor].radius;
        }
      }
      for( int meteor = 0; meteor < MAX_SMALL_METEORS; meteor++ )
      {
        if( smallMeteor[meteor].active )
        {
          // movement
          smallMeteor[meteor].position.x += smallMeteor[meteor].speed.x;
          smallMeteor[meteor].position.y += smallMeteor[meteor].speed.y;

          // collision with walls
          if( smallMeteor[meteor].position.x > screenWidth + smallMeteor[meteor].radius )
            smallMeteor[meteor].position.x = -(smallMeteor[meteor].radius);
          else if( smallMeteor[meteor].position.x < 0 - smallMeteor[meteor].radius )
            smallMeteor[meteor].position.x = screenWidth + smallMeteor[meteor].radius;
          if( smallMeteor[meteor].position.y > screenHeight + smallMeteor[meteor].radius )
            smallMeteor[meteor].position.y = -(smallMeteor[meteor].radius);
          else if( smallMeteor[meteor].position.y < 0 - smallMeteor[meteor].radius )
            smallMeteor[meteor].position.y = screenHeight + smallMeteor[meteor].radius;
        }
      }

      // collision logic: player bullets vs meteor
      for( int shots = 0; shots < PLAYER_MAX_SHOTS; shots++)
      {
        if( bullet[shots].active )
        {
          for( int meteor = 0; meteor < MAX_BIG_METEORS; meteor++ )
          {
            // check for big meteor collision with bullet
            if( bigMeteor[meteor].active && CheckCollisionCircles(bullet[shots].position, bullet[shots].radius, bigMeteor[meteor].position, bigMeteor[meteor].radius) )
            {
              bullet[shots].active = false;
              bullet[shots].lifeSpawn = 0;
              bigMeteor[shots].active = false;
              destroyedMeteorCount++;

              // split big meteor into medium meteors
              for( int splitMeteors = 0; splitMeteors < 2; splitMeteors++ )
              {
                if( midMeteorCount % 2 == 0 )
                {
                  midMeteor[midMeteorCount].position = (Vector2){ bigMeteor[meteor].position.x, bigMeteor[meteor].position.y };
                  midMeteor[midMeteorCount].speed = (Vector2){ cos(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED * -1, sin(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED * -1 };
                }
                else
                {
                  midMeteor[midMeteorCount].position = (Vector2){ bigMeteor[meteor].position.x, bigMeteor[meteor].position.y };
                  midMeteor[midMeteorCount].speed = (Vector2){ cos(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED, sin(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED };
                } 
                midMeteor[midMeteorCount].active = true;
                midMeteorCount++;
              }
              bigMeteor[meteor].color = RED;
              meteor = MAX_BIG_METEORS;
            }
          }

          // medium meteor collisions with bullets
          for( int meteor = 0; meteor < MAX_MEDIUM_METEORS; meteor++ )
          {
            if( midMeteor[meteor].active && CheckCollisionCircles(bullet[shots].position, bullet[shots].radius, midMeteor[meteor].position, midMeteor[meteor].radius) )
            {
              bullet[shots].active = false;
              bullet[shots].lifeSpawn = 0;
              midMeteor[meteor].active = false;
              destroyedMeteorCount++;

              for( int splitMeteors = 0; splitMeteors < 2; splitMeteors++ )
              {
                if( smallMeteorCount % 2 == 0 )
                {
                  smallMeteor[smallMeteorCount].position = (Vector2){ midMeteor[meteor].position.x, midMeteor[meteor].position.y };
                  smallMeteor[smallMeteorCount].speed = (Vector2){ cos(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED * -1, sin(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED * -1};
                }
                else
                {
                  smallMeteor[smallMeteorCount].position = (Vector2){ midMeteor[meteor].position.x, midMeteor[meteor].position.y };
                  smallMeteor[smallMeteorCount].speed = (Vector2){ cos(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED, sin(bullet[shots].rotation * DEG2RAD) * METEORS_SPEED };
                }
                smallMeteor[smallMeteorCount].active = true;
                smallMeteorCount++;
              }
              midMeteor[meteor].color = GREEN;
              meteor = MAX_MEDIUM_METEORS;
            }
          }

          // small meteor collision with bullets
          for( int meteor = 0; meteor < MAX_SMALL_METEORS; meteor++ )
          {
            if( smallMeteor[meteor].active && CheckCollisionCircles(bullet[shots].position, bullet[shots].radius, smallMeteor[meteor].position, smallMeteor[meteor].radius) )
            {
              bullet[shots].active = false;
              bullet[shots].lifeSpawn = 0;
              smallMeteor[meteor].active = false;
              destroyedMeteorCount++;
              smallMeteor[meteor].color = YELLOW;
              meteor = MAX_SMALL_METEORS;
            }
          }
        }
      }

    }
    if( destroyedMeteorCount == MAX_BIG_METEORS + MAX_MEDIUM_METEORS + MAX_SMALL_METEORS )
      victory = true;
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
  ClearBackground(RAYWHITE);

  if( !gameOver )
  {
    // draw spaceship
    Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * shipHeight, player.position.y - cosf(player.rotation * DEG2RAD) * shipHeight };
    Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
    Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (PLAYER_BASE_SIZE / 2) };
    DrawTriangle(v1, v2, v3, BLUE);

    // draw meteors
    for( int meteor = 0; meteor < MAX_BIG_METEORS; meteor++ )
    {
      if( bigMeteor[meteor].active )
        DrawCircleV(bigMeteor[meteor].position, bigMeteor[meteor].radius, DARKGRAY);
      else
        DrawCircleV(bigMeteor[meteor].position, bigMeteor[meteor].radius, Fade(LIGHTGRAY, 0.3f) );
    }
    for( int meteor = 0; meteor < MAX_MEDIUM_METEORS; meteor++ )
    {
      if( midMeteor[meteor].active )
        DrawCircleV(midMeteor[meteor].position, midMeteor[meteor].radius, GRAY);
      else
        DrawCircleV(midMeteor[meteor].position, midMeteor[meteor].radius, Fade(LIGHTGRAY, 0.3f) );
    }
    for( int meteor = 0; meteor < MAX_SMALL_METEORS; meteor++ )
    {
      if( smallMeteor[meteor].active )
        DrawCircleV(smallMeteor[meteor].position, smallMeteor[meteor].radius, GRAY);
      else
        DrawCircleV(smallMeteor[meteor].position, smallMeteor[meteor].radius, Fade(LIGHTGRAY, 0.3f) );
    }

    // draw bullets
    for( int shots = 0; shots < PLAYER_MAX_SHOTS; shots++ )
    {
      if( bullet[shots].active )
        DrawCircleV(bullet[shots].position, bullet[shots].radius, BLACK);
    }
    if( victory )
      DrawText("VICTORY", screenWidth / 2 - MeasureText("VICTORY", 20) / 2, screenHeight / 2 - 20, 20, LIGHTGRAY);
    if( pause )
      DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
  }
  else
    DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, GRAY);

  EndDrawing();
}

void UpdateDrawGame(void)
{}

void UnloadGame(void)
{
  UpdateGame();
  DrawGame();
}
