#include "ai.h"
#include <stdlib.h>
#include <limits.h>

#define MAX_DEPTH 3

static int directions[4][2] = { {1,0}, {0,1}, {1,1}, {1,-1} };

// 辅助函数：坐标合法性
static int is_valid(int row, int col) {
    return row >= 0 && row < BOARD_SIZE&& col >= 0 && col < BOARD_SIZE;
}

// 评估一行中的所有五元组组合
static int evaluate_line(Cell line[], int len, Cell self) {
    int score = 0;
    Cell opponent = (self == BLACK) ? WHITE : BLACK;

    for (int i = 0; i <= len - 5; ++i) {
        int self_count = 0, opp_count = 0, empty_count = 0;

        for (int j = 0; j < 5; ++j) {
            if (line[i + j] == self) self_count++;
            else if (line[i + j] == opponent) opp_count++;
            else empty_count++;
        }

        if (self_count > 0 && opp_count == 0) {
            switch (self_count) {
            case 5: score += 100000; break;
            case 4: score += (empty_count == 1) ? 10000 : 5000; break;
            case 3: score += (empty_count == 2) ? 1000 : 500; break;
            case 2: score += 100; break;
            case 1: score += 10; break;
            }
        }
        else if (opp_count > 0 && self_count == 0) {
            switch (opp_count) {
            case 5: score -= 100000; break;
            case 4: score -= (empty_count == 1) ? 9000 : 4000; break;
            case 3: score -= (empty_count == 2) ? 800 : 400; break;
            case 2: score -= 80; break;
            case 1: score -= 8; break;
            }
        }
    }
    return score;
}

// 评估整个棋盘
static int evaluate_board(GameState* game, Cell ai_piece) {
    int total_score = 0;
    Cell line[BOARD_SIZE];

    // 水平
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c)
            line[c] = game->board[r][c];
        total_score += evaluate_line(line, BOARD_SIZE, ai_piece);
    }

    // 垂直
    for (int c = 0; c < BOARD_SIZE; ++c) {
        for (int r = 0; r < BOARD_SIZE; ++r)
            line[r] = game->board[r][c];
        total_score += evaluate_line(line, BOARD_SIZE, ai_piece);
    }

    // 主对角线组
    for (int k = 0; k <= 2 * (BOARD_SIZE - 1); ++k) {
        int len = 0;
        for (int r = 0; r < BOARD_SIZE; ++r) {
            int c = k - r;
            if (c >= 0 && c < BOARD_SIZE)
                line[len++] = game->board[r][c];
        }
        if (len >= 5)
            total_score += evaluate_line(line, len, ai_piece);
    }

    // 副对角线组
    for (int k = -BOARD_SIZE + 1; k < BOARD_SIZE; ++k) {
        int len = 0;
        for (int r = 0; r < BOARD_SIZE; ++r) {
            int c = r - k;
            if (c >= 0 && c < BOARD_SIZE)
                line[len++] = game->board[r][c];
        }
        if (len >= 5)
            total_score += evaluate_line(line, len, ai_piece);
    }

    return total_score;
}

// Alpha-Beta 剪枝的 Minimax 搜索
static int minimax(GameState* game, int depth, int alpha, int beta,
    int maximizingPlayer, int* out_row, int* out_col, Cell ai_piece) {
    if (depth == 0) {
        return evaluate_board(game, ai_piece);
    }

    int best_score = maximizingPlayer ? INT_MIN : INT_MAX;
    int radius = 2;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (game->board[r][c] != EMPTY) continue;

            // 判断是否在已有棋子的邻域
            int near = 0;
            for (int dr = -radius; dr <= radius && !near; ++dr) {
                for (int dc = -radius; dc <= radius; ++dc) {
                    int nr = r + dr, nc = c + dc;
                    if (is_valid(nr, nc) && game->board[nr][nc] != EMPTY) {
                        near = 1;
                        break;
                    }
                }
            }
            if (!near) continue;

            Cell current_piece = maximizingPlayer ? ai_piece : (ai_piece == BLACK ? WHITE : BLACK);
            game->board[r][c] = current_piece;

            int win = check_win(game, r, c);
            int score = win ? (maximizingPlayer ? 1000000 : -1000000)
                : minimax(game, depth - 1, alpha, beta, !maximizingPlayer, NULL, NULL, ai_piece);

            game->board[r][c] = EMPTY;

            if (maximizingPlayer) {
                if (score > best_score) {
                    best_score = score;
                    if (out_row) *out_row = r;
                    if (out_col) *out_col = c;
                }
                if (score > alpha) alpha = score;
                if (beta <= alpha) return best_score;
            }
            else {
                if (score < best_score) best_score = score;
                if (score < beta) beta = score;
                if (beta <= alpha) return best_score;
            }
        }
    }

    return best_score;
}

// AI 落子接口
void ai_move(GameState* game, int* row, int* col) {
    Cell ai_piece = (game->turn == AI) ? WHITE : BLACK;
    minimax(game, MAX_DEPTH, INT_MIN, INT_MAX, 1, row, col, ai_piece);
}
