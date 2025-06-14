#include "../global.h"
#include <raylib.h>
#include <stdbool.h>
#include "functions.h"

void main_content(Vector2 mouse)
{
    int col = (int)((mouse.x - grid_origin.x) / TILE_SIZE);
    int row = (int)((mouse.y - grid_origin.y) / TILE_SIZE);

    if (col >= 0 && col < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
    {
        hovered_tile = (Vector2){col, row};

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 current_tile = (Vector2){col, row};

            if (selected_tile.x < 0)
            {
                selected_tile = current_tile;
            }
            else
            {
                if (are_tiles_adjacent(selected_tile, current_tile))
                {
                    swap_tiles((int)selected_tile.y, (int)selected_tile.x, (int)current_tile.y, (int)current_tile.x);

                    if (find_matches())
                    {
                        remove_matches();
                    }
                    else
                    {
                        // Swap back if no match
                        swap_tiles((int)selected_tile.y, (int)selected_tile.x, (int)current_tile.y, (int)current_tile.x);
                    }
                }

                selected_tile = (Vector2){-1, -1};
            }
        }
    }
    else
    {
        hovered_tile = (Vector2){-1, -1};
    }

    // Continuously find and remove matches if present
    if (find_matches())
    {
        remove_matches();
    }

    // Draw the entire board + highlights
    draw_board(mouse);
}

void reset_screen(Vector2 mouse)
{
    int centerX = GetScreenWidth() / 2;

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));

    DrawText("Time's up!", centerX - MeasureText("Time's up!", 40) / 2, GetScreenHeight() / 2 - 100, 40, RED);
    DrawText(TextFormat("Final Score: %d", score), centerX - 150, GetScreenHeight() / 2 - 40, 40, WHITE);
    DrawText("Click the button below to play again", centerX - 230, GetScreenHeight() / 2 + 10, 30, LIGHTGRAY);

    Rectangle restartBtn = {
        centerX - 100,
        GetScreenHeight() / 2 + 70,
        200,
        60};

    bool hovering = CheckCollisionPointRec(mouse, restartBtn);
    Color btnColor = hovering ? (Color){255, 80, 80, 255} : RED;

    DrawRectangleRec(restartBtn, btnColor);
    DrawRectangleLinesEx(restartBtn, 3, WHITE);

    const char *btnText = "Restart";
    int textWidth = MeasureText(btnText, 30);
    DrawText(btnText, restartBtn.x + (restartBtn.width - textWidth) / 2, restartBtn.y + 15, 30, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovering)
    {
        timer_seconds = TIMER_START;
        init_board();
        selected_tile = (Vector2){-1, -1};
        hovered_tile = (Vector2){-1, -1};
    }
}
