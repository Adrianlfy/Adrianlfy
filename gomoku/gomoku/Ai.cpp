#include "Ai.h"
#include <stdlib.h>


void ai_move(GameState* game, int* row, int* col) {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (game->board[r][c] == EMPTY) {
                *row = r;
                *col = c;
                return;
            }
        }
    }
    *row = 0; *col = 0;
}