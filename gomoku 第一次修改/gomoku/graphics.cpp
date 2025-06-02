/* === graphics.c === */
#include "graphics.h"
#include <SDL3/SDL.h>
int get_board_pos(int x, int y, int* row, int* col) {
    // 计算每个格子的大小
    int cell_size = WINDOW_WIDTH / BOARD_SIZE;

    // 计算行和列
    *col = x / cell_size;
    *row = y / cell_size;

    // 检查是否越界
    if (*row < 0 || *row >= BOARD_SIZE || *col < 0 || *col >= BOARD_SIZE) {
        return 0; // 越界，返回失败
    }

    return 1; // 成功，返回棋盘的行和列
}
void draw_board(SDL_Renderer* renderer, GameState* game) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 200, 255);  // 设置背景色为浅色
    SDL_RenderClear(renderer);  // 清空屏幕

    int cell_size = WINDOW_WIDTH / BOARD_SIZE;

    // 设置画线的颜色为黑色
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // 绘制棋盘网格
    for (int i = 0; i < BOARD_SIZE; ++i) {
        SDL_RenderLine(renderer, cell_size / 2, cell_size * i + cell_size / 2,
            WINDOW_WIDTH - cell_size / 2, cell_size * i + cell_size / 2);  // 横向线
        SDL_RenderLine(renderer, cell_size * i + cell_size / 2, cell_size / 2,
            cell_size * i + cell_size / 2, WINDOW_HEIGHT - cell_size / 2);  // 纵向线
    }

    // 绘制棋子
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (game->board[r][c] != EMPTY) {
                int x = c * cell_size + cell_size / 4;
                int y = r * cell_size + cell_size / 4;
                int radius = cell_size / 4;

                if (game->board[r][c] == BLACK) {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 黑色棋子
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // 白色棋子
                }

                // 绘制圆形棋子
                SDL_FRect rect = { (float)x, (float)y, (float)(2 * radius), (float)(2 * radius) };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);  // 更新渲染器，显示内容
}