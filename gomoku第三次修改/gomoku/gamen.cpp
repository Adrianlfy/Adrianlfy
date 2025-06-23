#include "game.h"
#include <string.h>

#include "game.h"
#include <string.h>

void init_game(GameState* game) {
    memset(game->board, EMPTY, sizeof(game->board));
    game->turn = HUMAN;
}

int make_move(GameState* game, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return 0;
    if (game->board[row][col] != EMPTY) return 0;
    game->board[row][col] = (game->turn == HUMAN) ? BLACK : WHITE;
    return 1;
}

int check_dir(Cell b[BOARD_SIZE][BOARD_SIZE], int r, int c, int dr, int dc, Cell color) {
    int count = 0;
    for (int i = -4; i <= 4; ++i) {
        int nr = r + i * dr, nc = c + i * dc;
        if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && b[nr][nc] == color) {
            count++;
            if (count >= 5) return 1;
        }
        else count = 0;
    }
    return 0;
}

int check_win(GameState* game, int row, int col) {
    Cell color = game->board[row][col];
    return check_dir(game->board, row, col, 1, 0, color) ||
        check_dir(game->board, row, col, 0, 1, color) ||
        check_dir(game->board, row, col, 1, 1, color) ||
        check_dir(game->board, row, col, 1, -1, color);
}