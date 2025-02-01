#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Set MSAA 4X hint before windows creation

    InitWindow(screenWidth, screenHeight, "raylib [core] example - gamepad information");

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (int i = 0, y = 5; i < 4; i++)     // MAX_GAMEPADS = 4
            {
                if (IsGamepadAvailable(i))
                {
                    DrawText(TextFormat("Gamepad name: %s", GetGamepadName(i)), 10, y, 10, BLACK);
                    y += 11;
                    DrawText(TextFormat("\tAxis count:   %d", GetGamepadAxisCount(i)), 10, y, 10, BLACK);
                    y += 11;

                    for (int axis = 0; axis < GetGamepadAxisCount(i); axis++)
                    {
                        DrawText(TextFormat("\tAxis %d = %f", axis, GetGamepadAxisMovement(i, axis)), 10, y, 10, BLACK);
                        y += 11;
                    }

                    for (int button = 0; button < 32; button++)
                    {
                        DrawText(TextFormat("\tButton %d = %d", button, IsGamepadButtonDown(i, button)), 10, y, 10, BLACK);
                        y += 11;
                    }
                }
            }

            DrawFPS(GetScreenWidth() - 100, 100);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
