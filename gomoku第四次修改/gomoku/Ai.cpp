
#include "ai.h"
#include <stdlib.h>
#include <limits.h>




#define MAX_DEPTH 4  // 

// 判断某位置附近是否存在棋子（加快搜索）
static int is_near_stone(GameState* game, int r, int c, int radius) {
    for (int dr = -radius; dr <= radius; dr++) {
        for (int dc = -radius; dc <= radius; dc++) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) continue;
            if (game->board[nr][nc] != EMPTY) return 1;
        }
    }
    return 0;
}

// 简单评分：AI 棋子数减去对手数
static int evaluate_board(GameState* game, Cell ai_piece) {
    Cell opponent = (ai_piece == BLACK) ? WHITE : BLACK;
    int ai_score = 0, opp_score = 0;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (game->board[r][c] == ai_piece) ai_score++;
            else if (game->board[r][c] == opponent) opp_score++;
        }
    }

    return ai_score - opp_score;
}

// 带 α-β 剪枝的 Minimax
static int minimax(GameState* game, int depth, int alpha, int beta, int maximizing, Cell ai_piece, int* best_row, int* best_col) {
    Cell opponent = (ai_piece == BLACK) ? WHITE : BLACK;

    if (depth == 0) {
        return evaluate_board(game, ai_piece);
    }

    int best_score = maximizing ? INT_MIN : INT_MAX;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (game->board[r][c] != EMPTY) continue;
            if (!is_near_stone(game, r, c, 2)) continue;

            // 模拟落子
            game->board[r][c] = maximizing ? ai_piece : opponent;

            int score = minimax(game, depth - 1, alpha, beta, !maximizing, ai_piece, NULL, NULL);

            // 撤销落子
            game->board[r][c] = EMPTY;

            if (maximizing) {
                if (score > best_score) {
                    best_score = score;
                    if (best_row && best_col) {
                        *best_row = r;
                        *best_col = c;
                    }
                }
                alpha = (score > alpha) ? score : alpha;
            }
            else {
                if (score < best_score) {
                    best_score = score;
                }
                beta = (score < beta) ? score : beta;
            }

            // 剪枝
            if (beta <= alpha) {
                return best_score;
            }
        }
    }

    return best_score;
}

// 公开接口：AI 落子决策
void ai_move(GameState* game, int* row, int* col) {
    Cell ai_piece = WHITE;  
    minimax(game, MAX_DEPTH, INT_MIN, INT_MAX, 1, ai_piece, row, col);
}
