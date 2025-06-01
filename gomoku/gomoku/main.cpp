#include <SDL3/SDL.h>
#include "game.h"
#include "graphics.h"
#include "ai.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Gomoku AI", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        return 1;
    }

    GameState game;
    init_game(&game);

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = 1;
            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && game.turn == HUMAN) {
                int row, col;
                if (get_board_pos(e.button.x, e.button.y, &row, &col)) {
                    if (make_move(&game, row, col)) {
                        if (check_win(&game, row, col)) {
                            SDL_Log("Human wins!");
                            quit = 1;
                        }
                        else {
                            game.turn = AI;
                        }
                    }
                }
            }
        }

        if (game.turn == AI) {
            int row, col;
            ai_move(&game, &row, &col);
            make_move(&game, row, col);
            if (check_win(&game, row, col)) {
                SDL_Log("AI wins!");
                quit = 1;
            }
            else {
                game.turn = HUMAN;
            }
        }

        draw_board(renderer, &game);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(50);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}