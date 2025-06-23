
#include "ai.h"
#include <stdlib.h>
#include <limits.h>


#define MAX_DEPTH 3  // 控制搜索深度，建议不超过 4

// 检查某位置附近 radius 范围内是否有棋子（用于范围剪枝）
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

// 简单评估函数：AI 落子数 - 对手落子数
static int evaluate_board(GameState* game, Cell ai_piece) {
    Cell opponent = (ai_piece == BLACK) ? WHITE : BLACK;
    int ai_count = 0, opp_count = 0;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (game->board[r][c] == ai_piece) ai_count++;
            else if (game->board[r][c] == opponent) opp_count++;
        }
    }

    return ai_count - opp_count;
}

// Minimax 核心：递归搜索评分最优步
static int minimax(GameState* game, int depth, int maximizing, Cell ai_piece, int* best_row, int* best_col) {
    Cell opponent = (ai_piece == BLACK) ? WHITE : BLACK;

    if (depth == 0) {
        return evaluate_board(game, ai_piece);
    }

    int best_score = maximizing ? INT_MIN : INT_MAX;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (game->board[r][c] != EMPTY) continue;
            if (!is_near_stone(game, r, c, 2)) continue; // 限定范围

            // 模拟落子
            game->board[r][c] = maximizing ? ai_piece : opponent;

            int score = minimax(game, depth - 1, !maximizing, ai_piece, NULL, NULL);

            // 撤销落子
            game->board[r][c] = EMPTY;

            // 更新最佳分数
            if (maximizing) {
                if (score > best_score) {
                    best_score = score;
                    if (best_row && best_col) {
                        *best_row = r;
                        *best_col = c;
                    }
                }
            }
            else {
                if (score < best_score) {
                    best_score = score;
                }
            }
        }
    }

    return best_score;
}

// AI 接口：选择最优位置
void ai_move(GameState* game, int* row, int* col) {
    Cell ai_piece = WHITE;  // 
    minimax(game, MAX_DEPTH, 1, ai_piece, row, col);
}
