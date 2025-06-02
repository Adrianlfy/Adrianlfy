#include "ai.h"
#include <stdlib.h>

static int score_line(Cell line[5], Cell ai_piece) {
    int ai_count = 0, opp_count = 0;
    Cell opponent = (ai_piece == BLACK) ? WHITE : BLACK;

    for (int i = 0; i < 5; i++) {
        if (line[i] == ai_piece) ai_count++;
        else if (line[i] == opponent) opp_count++;
    }

    if (ai_count > 0 && opp_count > 0) return 0; // 冲突行
    if (ai_count == 0 && opp_count == 0) return 1; // 空行
    if (ai_count == 4) return 1000;
    if (ai_count == 3) return 100;
    if (ai_count == 2) return 10;
    if (opp_count == 4) return 900;  // 高防守优先
    if (opp_count == 3) return 90;
    if (opp_count == 2) return 9;
    return 1;
}

static int evaluate(GameState* game, int row, int col, Cell ai_piece) {
    int total_score = 0;
    Cell backup = game->board[row][col];
    game->board[row][col] = ai_piece;

    // 横、竖、左斜、右斜方向检查
    const int dirs[4][2] = {
        {0, 1}, {1, 0}, {1, 1}, {1, -1}
    };

    for (int d = 0; d < 4; d++) {
        int dr = dirs[d][0], dc = dirs[d][1];
        for (int offset = -4; offset <= 0; offset++) {
            Cell line[5];
            int valid = 1;
            for (int k = 0; k < 5; k++) {
                int r = row + (offset + k) * dr;
                int c = col + (offset + k) * dc;
                if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) {
                    valid = 0; break;
                }
                line[k] = game->board[r][c];
            }
            if (valid)
                total_score += score_line(line, ai_piece);
        }
    }

    game->board[row][col] = backup;
    return total_score;
}

void ai_move(GameState* game, int* row, int* col) {
    int best_score = -1;
    int best_row = 0, best_col = 0;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (game->board[r][c] == EMPTY) {
                int score = evaluate(game, r, c, WHITE); 
                if (score > best_score) {
                    best_score = score;
                    best_row = r;
                    best_col = c;
                }
            }
        }
    }

    *row = best_row;
    *col = best_col;
}
