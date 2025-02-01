#include <raylib.h>
#include <raymath.h>

#define TILE_WIDTH 8
#define TILE_HEIGHT 8

void GameStartup();
void GameUpdate();
void GameRender();
void GameShutdown();
void DrawTile( int posX, int posY, int tile_index_x, int tile_index_y );

// screen dimensions
const int screenWidth = 800;
const int screenHeight = 600;

// texture
#define MAX_TEXTURES 1
typedef enum{ TEXTURE_TILEMAP = 0 } texture_asset;
Texture2D textures[MAX_TEXTURES];

#define MAX_SOUNDS 5
typedef enum
{
  SOUND_FOOT_GRASS = 0,
  SOUND_FOOT_STONE,
  SOUND_ATTACK,
  SOUND_DEATH,
  SOUND_COINS
} soundAsset;
Sound sounds[MAX_SOUNDS];

#define MAX_MUSIC 2
typedef enum
{
  MUSIC_LIGHT_AMBIENCE = 0,
  MUSIC_DARK_AMBIENCE
} music_asset;
Music music[MAX_MUSIC];

// world dimensions in tiles
#define WORLD_WIDTH 20
#define WORLD_HEIGHT 20

// tile
typedef enum
{
  TILE_TYPE_DIRT = 0,
  TILE_TYPE_GRASS,
  TILE_TYPE_TREE,
  TILE_TYPE_STONE
} tile_type; 
typedef struct
{
  int x;
  int y;
  int type;
} sTile;
sTile worldMap[WORLD_WIDTH][WORLD_HEIGHT];
sTile dungeonMap[WORLD_WIDTH][WORLD_HEIGHT];
typedef enum
{
  ZONE_ALL = 0,
  ZONE_WORLD,
  ZONE_DUNGEON
} eZone;

Camera2D camera = { 0 };

// entity 
typedef struct
{
  int x;
  int y;
  eZone zone;
  bool isAlive;
  bool isPassable;
  int health;
  int damage;
  int money;
  int experience;
} sEntity;
// player entity
sEntity player;
sEntity dungeonGate;
sEntity orc;
sEntity chest = { 0 };

// timer
typedef struct Timer {
    double startTime;   // Start time (seconds)
    double lifeTime;    // Lifetime (seconds)
    bool isActive;
} sTimer;
sTimer combatTextTimer;

void StartTimer(sTimer* timer, double lifetime);
bool IsTimerDone(sTimer timer);
double GetElapsed(sTimer timer);

int main(void)
{
  InitWindow(screenWidth, screenHeight, "Raylib 2D RPG");
  SetTargetFPS(60);
  
  GameStartup();

  while( !WindowShouldClose() )
  {
    GameUpdate();

    BeginDrawing();
      ClearBackground(GRAY);
      GameRender();
    EndDrawing();
  }
  GameShutdown();

  return 0;
}

void GameStartup()
{
  InitAudioDevice();
  
  // load texture asset
  Image image = LoadImage("assets/colored_tilemap_packed.png");
  textures[TEXTURE_TILEMAP] = LoadTextureFromImage(image);
  UnloadImage(image);

  // init world tilemap
  for( int column = 0; column < WORLD_WIDTH; column++ )
  {
    for( int row = 0; row < WORLD_HEIGHT; row++ )
    {
      worldMap[column][row] = (sTile){ .x = column, .y = row,
                                    .type = GetRandomValue(TILE_TYPE_DIRT, TILE_TYPE_TREE) };
      dungeonMap[column][row] = (sTile){ .x = column, .y = row,
                                         .type = TILE_TYPE_DIRT };
    }
  }
  // init player
  player = (sEntity){ .x = TILE_WIDTH * 3, .y = TILE_HEIGHT * 3, .zone = ZONE_WORLD,
                      .isAlive = true, .isPassable = false, .health = 100, .damage = 0,
                      .money = 1000, .experience = 0 };
  // init dungeon gate
  dungeonGate = (sEntity){ .x = TILE_WIDTH * 10, .y = TILE_HEIGHT * 10, .zone = ZONE_ALL,
                           .isPassable = true };
  // init orc
  orc = (sEntity){ .x = TILE_WIDTH * 5, .y = TILE_WIDTH * 5, .zone = ZONE_DUNGEON,
                   .isAlive = true, .isPassable = false, .health = 100, .damage = 0, .experience = GetRandomValue(10, 100) };

  //init camera2D
  camera = (Camera2D){ .target = (Vector2){ player.x, player.y },
             .offset = (Vector2){ (float)screenWidth / 2, (float)screenHeight / 2 },
             .rotation = 0.0f,
             .zoom = 3.0f };

  // init sound
  sounds[SOUND_FOOT_GRASS] = LoadSound("assets/Grass1.wav");
  sounds[SOUND_FOOT_STONE] = LoadSound("assets/Concrete1.wav");
  sounds[SOUND_ATTACK] = LoadSound("assets/07_human_atk_sword_2.wav");
  sounds[SOUND_DEATH] = LoadSound("assets/24_orc_death_spin.wav");
  sounds[SOUND_COINS] = LoadSound("assets/handleCoins.ogg");
  // init music
  music[MUSIC_LIGHT_AMBIENCE] = LoadMusicStream("assets/light-ambience.mp3");
  music[MUSIC_DARK_AMBIENCE] = LoadMusicStream("assets/dark-ambience.mp3");
  // play music
  PlayMusicStream(music[MUSIC_LIGHT_AMBIENCE] );
}
void GameUpdate()
{
  // music
  if( player.zone == ZONE_WORLD )
    UpdateMusicStream(music[MUSIC_LIGHT_AMBIENCE] );
  else
    UpdateMusicStream(music[MUSIC_DARK_AMBIENCE] );
  // player controls
  float x = player.x;
  float y = player.y;
  bool keyPressed = false;
  if( IsKeyPressed(KEY_LEFT) )
  {
    x -= 1 * TILE_WIDTH;
    keyPressed = true;
  }
  else if( IsKeyPressed(KEY_RIGHT) )
  {
    x += 1 * TILE_WIDTH;
    keyPressed = true;
  }
  else if( IsKeyPressed(KEY_UP) )
  {
    y -= 1 * TILE_HEIGHT;
    keyPressed = true;
  }
  else if( IsKeyPressed(KEY_DOWN) )
  {
    y += 1 * TILE_HEIGHT;
    keyPressed = true;
  }
  if( IsKeyPressed(KEY_E) )
  {
    if( player.x == dungeonGate.x && player.y == dungeonGate.y )
    {
      if( player.zone == ZONE_WORLD )
      {
        player.zone = ZONE_DUNGEON;
        StopMusicStream(music[MUSIC_LIGHT_AMBIENCE] );
        PlayMusicStream(music[MUSIC_DARK_AMBIENCE] );
      }
      else if( player.zone == ZONE_DUNGEON )
      {
        player.zone = ZONE_WORLD;
        StopMusicStream(music[MUSIC_DARK_AMBIENCE] );
        PlayMusicStream(music[MUSIC_LIGHT_AMBIENCE] );
      }
    }
  }
  if( IsKeyPressed(KEY_G) )
  {
    if( player.x == chest.x && player.y == chest.y )
    {
      chest.isAlive = false;
      player.money += chest.money;
      PlaySound(sounds[SOUND_COINS] );
    }
  }

  // zoom controls
  float wheel = GetMouseWheelMove();
  if( camera.zoom < 3 )
    camera.zoom = 3.0f;
  if( camera.zoom > 8 )
    camera.zoom = 8.0f;
  if( wheel != 0 )
  {
    const float zoomIncrement = 0.125f;
    camera.zoom += (wheel * zoomIncrement);
  }

  // check for orc collisions
  if( orc.isAlive && player.zone == orc.zone && 
      orc.x == x && orc.y == y )
  {
    int damage = GetRandomValue(2, 20);
    orc.health -= damage;
    orc.damage = damage;
    if( !combatTextTimer.isActive )
    {
      combatTextTimer.isActive = true;
      StartTimer(&combatTextTimer, 0.5f);
    }
    if( orc.health <= 0 )
    {
      PlaySound(sounds[SOUND_DEATH] );
      orc.isAlive = false;
      player.experience += orc.experience;
      chest = (sEntity){.x = orc.x, .y = orc.y, .isAlive = true, .zone = orc.zone, .money = GetRandomValue(10, 100) };
    }
    else
      PlaySound(sounds[SOUND_ATTACK] );
  }
  else
  {
    if( keyPressed )
    {
      if( player.zone == ZONE_WORLD )
        PlaySound(sounds[SOUND_FOOT_GRASS] );
      else if( player.zone == ZONE_DUNGEON )
        PlaySound(sounds[SOUND_FOOT_STONE] );
    }
    player.x = x;
    player.y = y;
    camera.target = (Vector2){ player.x, player.y };
  }

  if( IsTimerDone(combatTextTimer) )
    combatTextTimer.isActive = false;

}
void GameRender()
{
  sTile tile;
  int tile_index_x = 0;
  int tile_index_y = 0;

  BeginMode2D(camera);
    // render map
    for( int column = 0; column < WORLD_WIDTH; column++ )
    {
      for( int row = 0; row < WORLD_HEIGHT; row++ )
      {
        if( player.zone == ZONE_WORLD )
          tile = worldMap[column][row];
        else if( player.zone == ZONE_DUNGEON )
          tile = dungeonMap[column][row];
        switch( tile.type )
        {
          case TILE_TYPE_DIRT:
            tile_index_x = 4;
            tile_index_y = 4;
            break;
          case TILE_TYPE_GRASS:
            tile_index_x = 5;
            tile_index_y = 4;
            break;
          case TILE_TYPE_TREE:
            tile_index_x = 5;
            tile_index_y = 5;
            break;
        }
        DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, tile_index_x, tile_index_y);
      }
    }
    // render dungeon gate
    DrawTile(dungeonGate.x, dungeonGate.y, 8, 9);
    // render orc
    if( orc.zone == player.zone )
    {
      if( orc.isAlive )
        DrawTile(orc.x, orc.y, 11, 0);
      if( combatTextTimer.isActive )
        DrawText(TextFormat("%d", orc.damage), orc.x, orc.y - 10, 9, YELLOW);
      if( chest.isAlive )
        DrawTile(chest.x, chest.y, 9, 3);
    }
    // render player
    DrawTile(camera.target.x, camera.target.y, 4, 0);
  EndMode2D();

  DrawRectangle(5, 5, 330, 120, Fade(SKYBLUE, 0.5f) );
  DrawRectangleLines(5, 5, 330, 120, BLUE);

  DrawText(TextFormat("Camera Target: (%06.2f, %06.2f)", camera.target.x, camera.target.y), 15, 10, 14, YELLOW);
  DrawText(TextFormat("Camera Zoom: %06.2f", camera.zoom), 15, 30, 14, YELLOW);
  DrawText(TextFormat("Player Health: %d", player.health), 15, 50, 14, YELLOW);
  DrawText(TextFormat("Player XP: %d", player.experience), 15, 70, 14, YELLOW);
  DrawText(TextFormat("Player Money: %d", player.money), 15, 90, 14, YELLOW);
  if( orc.isAlive )
    DrawText(TextFormat("Orc Health: %d", orc.health), 15, 110, 14, YELLOW);
}
void GameShutdown()
{
  for( int texture = 0; texture < MAX_TEXTURES; texture++ )
    UnloadTexture( textures[texture] );

  for( int sound = 0; sound < MAX_SOUNDS; sound++ )
    UnloadSound(sounds[sound] );
  for( int song = 0; song < MAX_MUSIC; song++ )
  {
    StopMusicStream(music[song] );
    UnloadMusicStream(music[song] );
  }

  CloseAudioDevice();
  CloseWindow();
}

void DrawTile( int posX, int posY, int tile_index_x, int tile_index_y )
{
  Rectangle sourceRect = { (float)tile_index_x * TILE_WIDTH, (float)tile_index_y * TILE_HEIGHT, (float)TILE_WIDTH, (float)TILE_HEIGHT };
  Rectangle destRect = { (float)posX, (float)posY, (float)TILE_WIDTH, (float)TILE_HEIGHT };
  Vector2 origin = { 0 , 0 };
  DrawTexturePro(textures[TEXTURE_TILEMAP], sourceRect, destRect, origin, 0.0f, WHITE);
}
void StartTimer(sTimer* timer, double lifetime)
{
    timer->startTime = GetTime();
    timer->lifeTime = lifetime;
}

bool IsTimerDone(sTimer timer)
{
    return GetTime() - timer.startTime >= timer.lifeTime;
}

double GetElapsed(sTimer timer)
{
    return GetTime() - timer.startTime;
}
