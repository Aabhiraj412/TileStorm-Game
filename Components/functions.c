#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include "../global.h"
#include "functions.h"

#define HIGHSCORE_FILE ".highscore"

char random_tile(void)
{
    return tile_chars[rand() % TILE_TYPES]; 
}

bool find_matches()
{
    bool found = false;

    // Clear previous matches
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            matches[i][j] = false;

    // Horizontal matches
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            char t = board[i][j];
            if (t != ' ' && t == board[i][j + 1] && t == board[i][j + 2])
            {
                matches[i][j] = true;
                matches[i][j + 1] = true;
                matches[i][j + 2] = true;
                found = true;
            }
        }
    }

    // Vertical matches
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        for (int i = 0; i < BOARD_SIZE - 2; i++)
        {
            char t = board[i][j];
            if (t != ' ' && t == board[i + 1][j] && t == board[i + 2][j])
            {
                matches[i][j] = true;
                matches[i + 1][j] = true;
                matches[i + 2][j] = true;
                found = true;
            }
        }
    }

    return found;
}

void remove_matches()
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        int write_row = BOARD_SIZE - 1;

        // Move non-matched tiles down
        for (int row = BOARD_SIZE - 1; row >= 0; row--)
        {
            if (!matches[row][col])
            {
                if (row != write_row)
                {
                    board[write_row][col] = board[row][col];
                    fall_offset[write_row][col] = (write_row - row) * TILE_SIZE;
                    board[row][col] = ' ';
                }
                write_row--;
            }
        }

        // Fill new tiles at top
        while (write_row >= 0)
        {
            board[write_row][col] = random_tile();
            fall_offset[write_row][col] = (write_row + 1) * TILE_SIZE;
            write_row--;
        }
    }

    // Increase score
    int matched_tiles = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (matches[i][j])
                matched_tiles++;

    score += matched_tiles * 10;
}

void swap_tiles(int row1, int col1, int row2, int col2)
{
    char temp = board[row1][col1];
    board[row1][col1] = board[row2][col2];
    board[row2][col2] = temp;

    fall_offset[row1][col1] = 0;
    fall_offset[row2][col2] = 0;
}

bool are_tiles_adjacent(Vector2 tile1, Vector2 tile2)
{
    return (abs(tile1.x - tile2.x) == 1 && tile1.y == tile2.y) || (abs(tile1.y - tile2.y) == 1 && tile1.x == tile2.x);
}

void init_board()
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = random_tile();

    while (find_matches())
    {
        remove_matches();
    }

    int gridWidth = BOARD_SIZE * TILE_SIZE;
    int gridHeight = BOARD_SIZE * TILE_SIZE;

    grid_origin = (Vector2){
        (GetScreenWidth() - gridWidth) / 2.0f,
        (GetScreenHeight() - gridHeight) / 2.0f};

    score = 0;
}

// Smoothly reduce fall offsets each frame to animate falling
void apply_gravity(void)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (fall_offset[i][j] > 0)
            {
                fall_offset[i][j] -= fall_speed;
                if (fall_offset[i][j] < 0)
                    fall_offset[i][j] = 0;
            }
        }
    }
}

// Draw a single tile with fall offset applied and highlight if matched
void draw_tile(int row, int col)
{
    Rectangle tileRect = {
        grid_origin.x + col * TILE_SIZE,
        grid_origin.y + row * TILE_SIZE - fall_offset[row][col],
        TILE_SIZE,
        TILE_SIZE};

    float roundness = 0.15f;

    // Draw tile background with rounded edges
    DrawRectangleRounded(tileRect, roundness, 4, DARKGRAY);

    // Draw tile border lines with rounded edges
    DrawRectangleRoundedLines(tileRect, roundness, 4, WHITE);

    // Draw tile letter if not empty
    if (board[row][col] != ' ')
    {
        DrawTextEx(GetFontDefault(),
                   TextFormat("%c", board[row][col]),
                   (Vector2){tileRect.x + TILE_SIZE / 3, tileRect.y + TILE_SIZE / 4},
                   50,
                   1,
                   board[row][col] == '@' ? GREEN : board[row][col] == '#' ? RED : board[row][col] == '$' ? BLUE : board[row][col] == '%' ? YELLOW :  PURPLE);
    }
}

void draw_hover_highlight(Vector2 hovered_tile)
{
    if (hovered_tile.x < 0 || hovered_tile.y < 0)
        return;

    Rectangle hoverRect = {
        grid_origin.x + hovered_tile.x * TILE_SIZE,
        grid_origin.y + hovered_tile.y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE};

    DrawRectangleLinesEx(hoverRect, 4, YELLOW);
}

void draw_selected_highlight(Vector2 selected_tile)
{
    if (selected_tile.x < 0 || selected_tile.y < 0)
        return;

    Rectangle selectedRect = {
        grid_origin.x + selected_tile.x * TILE_SIZE,
        grid_origin.y + selected_tile.y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE};

    DrawRectangleRec(selectedRect, (Color){255, 106, 111, 180}); // translucent pink-red
    DrawRectangleLinesEx(selectedRect, 4, WHITE);

    DrawTextEx(GetFontDefault(),
               TextFormat("%c", board[(int)selected_tile.y][(int)selected_tile.x]),
               (Vector2){selectedRect.x + TILE_SIZE / 3, selectedRect.y + TILE_SIZE / 4},
               50,
               1,
               WHITE);
}

void draw_board(Vector2 mouse)
{
    apply_gravity();

    // Draw all tiles
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            draw_tile(row, col);
        }
    }

    // Draw highlights on top
    draw_hover_highlight(hovered_tile);
    draw_selected_highlight(selected_tile);
}

void save_high_score(int score)
{
    FILE *file = fopen(HIGHSCORE_FILE, "w");
    if (file != NULL)
    {
        fprintf(file, "%d", score);
        fclose(file);
    }
}

int load_high_score()
{
    FILE *file = fopen(HIGHSCORE_FILE, "r");
    int score = 0;
    if (file != NULL)
    {
        fscanf(file, "%d", &score);
        fclose(file);
    }
    return score;
}
