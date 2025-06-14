#include "global.h"
#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include "Components/functions.h"

// const char *tile_chars[TILE_TYPES] = {"♦", "♣", "♠", "★", "☯"};
const char tile_chars[TILE_TYPES] = {'@', '#', '$', '%', '&'};
// const int TIMER_START = 30;

char board[BOARD_SIZE][BOARD_SIZE];
// int board[BOARD_SIZE][BOARD_SIZE] = {-1}; // Using integers for tile types
bool matches[BOARD_SIZE][BOARD_SIZE] = {0};
float fall_offset[BOARD_SIZE][BOARD_SIZE] = {0};

int score = 0;
int high_score = 0;

Vector2 grid_origin = {0};
Texture2D bg = {0};

Vector2 hovered_tile = {-1, -1};
Vector2 selected_tile = {-1, -1};

float fall_speed = 16.0f;

int timer_seconds = TIMER_START;
double last_time = 0.0;

int main(void)
{
    const int screenWidth = GetMonitorWidth(0);
    const int screenHeight = GetMonitorHeight(0);

    InitWindow(screenWidth, screenHeight, "Tile Board with Timer and Restart");

    SetTargetFPS(60);
    srand((unsigned int)time(NULL));

    high_score = load_high_score(); // Load high score from file

    bg = LoadTexture("Assets/123.jpg");

    init_board();

    Vector2 mouse = {0, 0};

    last_time = GetTime();

    Rectangle restartBtn = {20, screenHeight - 70, 180, 50}; // Button at bottom left

    while (!WindowShouldClose())
    {
        // Timer logic - count down every second
        double current_time = GetTime();
        if (current_time - last_time >= 1.0)
        {
            last_time = current_time;
            if (timer_seconds > 0)
            {
                timer_seconds--;
            }
        }

        mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexturePro(
            bg,
            (Rectangle){0, 0, (float)bg.width, (float)bg.height},
            (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            (Vector2){0, 0},
            0.0f,
            WHITE);

        DrawRectangle(
            grid_origin.x,
            grid_origin.y,
            BOARD_SIZE * TILE_SIZE,
            BOARD_SIZE * TILE_SIZE,
            Fade(DARKGRAY, 0.5f));

        if (timer_seconds > 0)
        {
            main_content(mouse);
        }
        else
        {
            reset_screen(mouse);
        }

        // Draw Score and Timer text
        DrawText(TextFormat("Score: %d", score), 10, 10, 40, WHITE);
        if (score > high_score)
        {
            high_score = score;          // Update high score if current score is higher
            save_high_score(high_score); // Save new high score
        }
        DrawText(TextFormat("High Score: %d", high_score), 10, 60, 40, WHITE);
        DrawText(TextFormat("Time: %d", timer_seconds), GetScreenWidth() - 180, 10, 40, WHITE);

        EndDrawing();
    }

    UnloadTexture(bg);
    CloseWindow();

    return 0;
}
