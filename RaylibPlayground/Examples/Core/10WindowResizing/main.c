#include <raylib.h>
#include <raymath.h>

// global variables
#define windowWidth 800
#define windowHeight 450

// modular functions
#define MAX( a, b ) ( (a) > (b) ? (a) : (b) )
#define MIN( a, b ) ( (a) < (b) ? (a) : (b) )

int main(void)
{
  // flags to enable
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT );
  InitWindow(windowWidth, windowHeight, "Window Resizing");
  SetWindowMinSize(windowWidth, windowHeight);

  int gameScreenWidth = 640;
  int gameScreenHeight = 480;

  // render texture init, used to hold rendering result to easily resize
  RenderTexture2D target = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
  SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

  Color colors[10] = { 0 };
  for( int colorIndex = 0; colorIndex < 10; colorIndex++ )
    colors[colorIndex] = (Color){GetRandomValue(100, 250), GetRandomValue(50, 150), GetRandomValue(10, 100), 255 };

  SetTargetFPS(60);

// Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Compute required framebuffer scaling
        float scale = MIN((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);

        if (IsKeyPressed(KEY_SPACE))
        {
            // Recalculate random colors for the bars
            for (int i = 0; i < 10; i++) colors[i] = (Color){ GetRandomValue(100, 250), GetRandomValue(50, 150), GetRandomValue(10, 100), 255 };
        }

        // Update virtual mouse (clamped mouse value behind game screen)
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse = { 0 };
        virtualMouse.x = (mouse.x - (GetScreenWidth() - (gameScreenWidth*scale))*0.5f)/scale;
        virtualMouse.y = (mouse.y - (GetScreenHeight() - (gameScreenHeight*scale))*0.5f)/scale;
        virtualMouse = Vector2Clamp(virtualMouse, (Vector2){ 0, 0 }, (Vector2){ (float)gameScreenWidth, (float)gameScreenHeight });

        // Apply the same transformation as the virtual mouse to the real mouse (i.e. to work with raygui)
        //SetMouseOffset(-(GetScreenWidth() - (gameScreenWidth*scale))*0.5f, -(GetScreenHeight() - (gameScreenHeight*scale))*0.5f);
        //SetMouseScale(1/scale, 1/scale);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // Draw everything in the render texture, note this will not be rendered on screen, yet
        BeginTextureMode(target);
            ClearBackground(RAYWHITE);  // Clear render texture background color

            for (int i = 0; i < 10; i++) DrawRectangle(0, (gameScreenHeight/10)*i, gameScreenWidth, gameScreenHeight/10, colors[i]);

            DrawText("If executed inside a window,\nyou can resize the window,\nand see the screen scaling!", 10, 25, 20, WHITE);
            DrawText(TextFormat("Default Mouse: [%i , %i]", (int)mouse.x, (int)mouse.y), 350, 25, 20, GREEN);
            DrawText(TextFormat("Virtual Mouse: [%i , %i]", (int)virtualMouse.x, (int)virtualMouse.y), 350, 55, 20, YELLOW);
        EndTextureMode();
        
        BeginDrawing();
            ClearBackground(BLACK);     // Clear screen background

            // Draw render texture to screen, properly scaled
            DrawTexturePro(target.texture, (Rectangle){ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height },
                           (Rectangle){ (GetScreenWidth() - ((float)gameScreenWidth*scale))*0.5f, (GetScreenHeight() - ((float)gameScreenHeight*scale))*0.5f,
                           (float)gameScreenWidth*scale, (float)gameScreenHeight*scale }, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
        //--------------------------------------------------------------------------------------
    }
  UnloadRenderTexture(target);
  CloseWindow();

  return 0;
}
