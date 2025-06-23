#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 15

typedef enum { EMPTY, BLACK, WHITE } Cell;
typedef enum { HUMAN, AI } Player;

typedef struct {
    Cell board[BOARD_SIZE][BOARD_SIZE];
    Player turn;
} GameState;

void init_game(GameState* game);
int make_move(GameState* game, int row, int col);
int check_win(GameState* game, int row, int col);

#endif

