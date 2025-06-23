
#include "ai.h"
#include <stdlib.h>
#include <limits.h>

#define MAX_DEPTH 2

static int evaluate_board(GameState* game, Cell ai_piece);
static int minimax(GameState* game, int depth, int maximizing,  Cell ai_piece, int* best_row, int* best_col);

// 调用接口
void ai_move(GameState* game, int* row, int* col) {
    minimax(game, MAX_DEPTH, 1, WHITE, row, col);  // 假设 AI 是 WHITE
}

static int score_line(Cell line[5], Cell ai_piece) {
    int ai_count = 0, opp_count = 0;
    Cell opp = (ai_piece == BLACK) ? WHITE : BLACK;

    for (int i = 0; i < 5; i++) {
        if (line[i] == ai_piece) ai_count++;
        else if (line[i] == opp) opp_count++;
    }

    if (ai_count > 0 && opp_count > 0) return 0;
    if (ai_count == 5) return 100000;
    if (ai_count == 4) return 1000;
    if (ai_count == 3) return 100;
    if (opp_count == 4) return 900;
    if (opp_count == 3) return 90;
    return ai_count + opp_count;
}

// 简单评估整个棋盘分数
static int evaluate_board(GameState* game, Cell ai_piece) {
    int score = 0;
    const int dirs[4][2] = { {0,1},{1,0},{1,1},{1,-1} };

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            for (int d = 0; d < 4; d++) {
                Cell line[5];
                int valid = 1;
                for (int k = 0; k < 5; k++) {
                    int nr = r + k * dirs[d][0];
                    int nc = c + k * dirs[d][1];
                    if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) {
                        valid = 0;
                        break;
                    }
                    line[k] = game->board[nr][nc];
                }
                if (valid) {
                    score += score_line(line, ai_piece);
                }
            }
        }
    }
    return score;
}

// Minimax 核心函数
static int minimax(GameState* game, int depth, int maximizing, Cell ai_piece, int* best_row, int* best_col) {
    Cell opponent = (ai_piece == BLACK) ? WHITE : BLACK;

    if (depth == 0) {
        return evaluate_board(game, ai_piece);
    }

    int best_score = maximizing ? INT_MIN : INT_MAX;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (game->board[r][c] != EMPTY) continue;

            game->board[r][c] = maximizing ? ai_piece : opponent;
            int score = minimax(game, depth - 1, !maximizing, ai_piece, NULL, NULL);
            game->board[r][c] = EMPTY;

            if (maximizing && score > best_score) {
                best_score = score;
                if (best_row && best_col) {
                    *best_row = r;
                    *best_col = c;
                }
            }
            else if (!maximizing && score < best_score) {
                best_score = score;
            }
        }
    }

    return best_score;
}