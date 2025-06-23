#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <SDL3/SDL.h>
#include "game.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

// 修改为使用 SDL_Renderer
void draw_board(SDL_Renderer* renderer, GameState* game);
int get_board_pos(int x, int y, int* row, int* col);

#endif