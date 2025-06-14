#pragma once
#include <raylib.h>
#include <stdbool.h>

char random_tile(void);

bool find_matches(void);
void remove_matches(void);

void swap_tiles(int row1, int col1, int row2, int col2);
bool are_tiles_adjacent(Vector2 tile1, Vector2 tile2);

void init_board(void);
void apply_gravity(void);

void draw_tile(int row, int col);
void draw_hover_highlight(Vector2 hovered_tile);
void draw_selected_highlight(Vector2 selected_tile);
void draw_board(Vector2 mouse);

void save_high_score(int score);
int load_high_score(void);
