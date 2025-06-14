#pragma once
#include <raylib.h>
#include <stdbool.h>

#define BOARD_SIZE 8
#define TILE_SIZE 80
#define TILE_TYPES 5
#define TIMER_START 60

// extern const char *tile_chars[TILE_TYPES];

// Board and state
extern char board[BOARD_SIZE][BOARD_SIZE];
// extern int board[BOARD_SIZE][BOARD_SIZE];
extern bool matches[BOARD_SIZE][BOARD_SIZE];
extern float fall_offset[BOARD_SIZE][BOARD_SIZE];

extern Vector2 selected_tile;
extern Vector2 hovered_tile;
extern Vector2 grid_origin;

extern int score;
extern int timer_seconds;
extern float fall_speed;
extern const char tile_chars[];

// Game logic functions
void init_board(void);
void draw_board(Vector2 mouse);
bool are_tiles_adjacent(Vector2 a, Vector2 b);
void swap_tiles(int row1, int col1, int row2, int col2);
bool find_matches(void);
void remove_matches(void);
void apply_gravity(void);

// Screen drawing functions
void main_content(Vector2 mouse);
void reset_screen(Vector2 mouse);
