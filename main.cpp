#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <string>

#define BOARD_SIZE 8
#define TILE_SIZE 100
#define TILE_TYPES 5

const char tile_chars[TILE_TYPES] = {'@', '#', '$', '%', '&'};

char board[BOARD_SIZE][BOARD_SIZE];
bool matches[BOARD_SIZE][BOARD_SIZE] = {0};
float fall_offset[BOARD_SIZE][BOARD_SIZE] = {0};

int score = 0;
int high_score = 0;
Vector2 grid_origin;
Texture2D bg;
Vector2 hovered_tile = {-1, -1};
Vector2 selected_tile = {-1, -1};
float fall_speed = 16.0f;

const int TIMER_START = 60;
int timer_seconds = TIMER_START;
double last_time = 0.0;

char RandomTile()
{
    return tile_chars[GetRandomValue(0, TILE_TYPES - 1)];
}

void LoadHighScore()
{
    std::ifstream file(".highscore");
    if (file.is_open())
        file >> high_score;
    file.close();
}

void SaveHighScore()
{
    std::ofstream file(".highscore");
    if (file.is_open())
        file << high_score;
    file.close();
}

bool FindMatches()
{
    bool found = false;
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            matches[i][j] = false;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 2; j++)
        {
            char t = board[i][j];
            if (t != ' ' && t == board[i][j + 1] && t == board[i][j + 2])
            {
                matches[i][j] = matches[i][j + 1] = matches[i][j + 2] = true;
                found = true;
            }
        }
    }

    for (int j = 0; j < BOARD_SIZE; j++)
    {
        for (int i = 0; i < BOARD_SIZE - 2; i++)
        {
            char t = board[i][j];
            if (t != ' ' && t == board[i + 1][j] && t == board[i + 2][j])
            {
                matches[i][j] = matches[i + 1][j] = matches[i + 2][j] = true;
                found = true;
            }
        }
    }

    return found;
}

void RemoveMatches()
{
    for (int col = 0; col < BOARD_SIZE; col++)
    {
        int write_row = BOARD_SIZE - 1;

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

        while (write_row >= 0)
        {
            board[write_row][col] = RandomTile();
            fall_offset[write_row][col] = (write_row + 1) * TILE_SIZE;
            write_row--;
        }
    }

    int matched_tiles = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (matches[i][j])
                matched_tiles++;

    score += matched_tiles * 10;
    if (score > high_score)
    {
        high_score = score;
        SaveHighScore();
    }
}

void SwapTiles(int r1, int c1, int r2, int c2)
{
    std::swap(board[r1][c1], board[r2][c2]);
    fall_offset[r1][c1] = fall_offset[r2][c2] = 0;
}

bool AreAdjacent(Vector2 a, Vector2 b)
{
    return (std::abs((int)a.x - (int)b.x) == 1 && a.y == b.y) ||
           (std::abs((int)a.y - (int)b.y) == 1 && a.x == b.x);
}

void InitBoard()
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board[i][j] = RandomTile();

    while (FindMatches())
        RemoveMatches();

    grid_origin = {
        (float)(GetScreenWidth() - BOARD_SIZE * TILE_SIZE) / 2.0f,
        (float)(GetScreenHeight() - BOARD_SIZE * TILE_SIZE) / 2.0f};

    score = 0;
}

void DrawBoard()
{
    for (int r = 0; r < BOARD_SIZE; r++)
    {
        for (int c = 0; c < BOARD_SIZE; c++)
        {
            float x = grid_origin.x + c * TILE_SIZE;
            float y = grid_origin.y + r * TILE_SIZE - fall_offset[r][c];
            Rectangle tileRect = {x, y, TILE_SIZE, TILE_SIZE};

            // Hover highlight
            if ((int)hovered_tile.x == c && (int)hovered_tile.y == r)
                DrawRectangleRec(tileRect, Fade(SKYBLUE, 0.3f));

            // Selected highlight
            if ((int)selected_tile.x == c && (int)selected_tile.y == r)
                DrawRectangleRec(tileRect, Fade(RED, 0.2f));

            // Tile background base
            DrawRectangleRounded((Rectangle){x + 5, y + 5, TILE_SIZE - 10, TILE_SIZE - 10}, 0.3f, 6, Fade(LIGHTGRAY, 0.2f));
            DrawRectangleRounded((Rectangle){x + 9, y + 9, TILE_SIZE - 10, TILE_SIZE - 10}, 0.3f, 6, Fade(BLACK, 0.2f));

            // Tile symbol
            if (board[r][c] != ' ')
            {
                Color color = board[r][c] == '@' ? GREEN : board[r][c] == '#' ? RED
                                                       : board[r][c] == '$'   ? BLUE
                                                       : board[r][c] == '%'   ? YELLOW
                                                                              : PURPLE;

                for (int i = 0; i < 3; i++)
                    DrawTextEx(GetFontDefault(),
                               TextFormat("%c", board[r][c]),
                               (Vector2){x + TILE_SIZE / 3 - i, y + TILE_SIZE / 4 - i},
                               50, 1, Fade(color, 0.2f));

                DrawTextEx(GetFontDefault(),
                           TextFormat("%c", board[r][c]),
                           (Vector2){x + TILE_SIZE / 3, y + TILE_SIZE / 4},
                           50, 1, color);
            }
        }
    }

    // Update falling animation
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            if (fall_offset[i][j] > 0)
                fall_offset[i][j] = std::max(0.0f, fall_offset[i][j] - fall_speed);
}

void GameLogic(Vector2 mouse)
{
    int c = (mouse.x - grid_origin.x) / TILE_SIZE;
    int r = (mouse.y - grid_origin.y) / TILE_SIZE;

    if (c >= 0 && c < BOARD_SIZE && r >= 0 && r < BOARD_SIZE)
    {
        hovered_tile = {(float)c, (float)r};

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (selected_tile.x < 0)
                selected_tile = hovered_tile;
            else
            {
                if (AreAdjacent(selected_tile, hovered_tile))
                {
                    SwapTiles((int)selected_tile.y, (int)selected_tile.x, r, c);
                    if (!FindMatches())
                        SwapTiles((int)selected_tile.y, (int)selected_tile.x, r, c);
                    else
                        RemoveMatches();
                }
                selected_tile = {-1, -1};
            }
        }
    }
    else
        hovered_tile = {-1, -1};

    if (FindMatches())
        RemoveMatches();

    DrawBoard();

    if (selected_tile.x >= 0 && selected_tile.y >= 0)
    {
        Rectangle sel = {
            grid_origin.x + selected_tile.x * TILE_SIZE,
            grid_origin.y + selected_tile.y * TILE_SIZE,
            TILE_SIZE, TILE_SIZE};
        DrawRectangleLinesEx(sel, 4, WHITE);
        DrawRectangleLinesEx((Rectangle){sel.x + 2, sel.y + 2, sel.width - 4, sel.height - 4}, 2, GOLD);
    }
}

void DrawRestartScreen(Vector2 mouse)
{
    int centerX = GetScreenWidth() / 2;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
    DrawText("Time's up!", centerX - MeasureText("Time's up!", 50) / 2, GetScreenHeight() / 2 - 120, 50, RED);
    DrawText(TextFormat("Final Score: %d", score), centerX - 150, GetScreenHeight() / 2 - 50, 40, WHITE);
    DrawText("Click Restart to play again", centerX - 180, GetScreenHeight() / 2 + 10, 28, LIGHTGRAY);

    Rectangle btn = {(float)(centerX - 120), (float)(GetScreenHeight() / 2 + 70), 240, 65};
    bool hovering = CheckCollisionPointRec(mouse, btn);
    Color btnColor = hovering ? Fade(ORANGE, 0.9f) : DARKGRAY;

    DrawRectangleRounded(btn, 0.4f, 8, btnColor);
    // DrawRectangleRoundedLines(btn, 0.4f, 8, 3.0f, WHITE);
    DrawRectangleRoundedLinesEx(btn, 0.4f, 8, 3.0f, WHITE);

    DrawText("Restart", btn.x + 50, btn.y + 18, 32, WHITE);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovering)
    {
        timer_seconds = TIMER_START;
        InitBoard();
        selected_tile = hovered_tile = {-1, -1};
    }
}

int main()
{
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "Tile Match Game - C++ Raylib");
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(NULL));

    bg = LoadTexture("Assets/123.jpg");
    LoadHighScore();
    InitBoard();
    last_time = GetTime();

    while (!WindowShouldClose())
    {
        double current_time = GetTime();
        if (current_time - last_time >= 1.0)
        {
            last_time = current_time;
            if (timer_seconds > 0)
                timer_seconds--;
        }

        Vector2 mouse = GetMousePosition();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(bg, {0, 0, (float)bg.width, (float)bg.height}, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0, WHITE);
        DrawRectangle(grid_origin.x, grid_origin.y, TILE_SIZE * BOARD_SIZE, TILE_SIZE * BOARD_SIZE, Fade(DARKGRAY, 0.4f));

        if (timer_seconds > 0)
            GameLogic(mouse);
        else
            DrawRestartScreen(mouse);

        int textX = 20;
        DrawText(TextFormat("Score: %d", score), textX + 2, 22, 30, Fade(BLACK, 0.5f));
        DrawText(TextFormat("Score: %d", score), textX, 20, 30, WHITE);
        DrawText(TextFormat("High Score: %d", high_score), textX, 60, 30, YELLOW);
        DrawText(TextFormat("Time: %d", timer_seconds), GetScreenWidth() - 200, 20, 30, timer_seconds < 10 ? RED : LIGHTGRAY);

        EndDrawing();
    }

    UnloadTexture(bg);
    CloseWindow();
    return 0;
}
