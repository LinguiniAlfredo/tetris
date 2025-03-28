#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "board.h"
#include "ui/hud.h"
#include "utils/timer.h"
#include "entities/tetromino.h"

using namespace std;

const int BOARD_WIDTH_PX = 10 * 8;
const int BOARD_HEIGHT_PX = 20 * 8;

SDL_Renderer *renderer = nullptr;
SDL_Window *window = nullptr;

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		printf("SDL initialized successfully\n");
		window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_WIDTH_PX*2.3, BOARD_HEIGHT_PX*2.3, SDL_WINDOW_SHOWN);
		if (window == nullptr) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		} else {
            printf("window created successfully\n");
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == nullptr) {
				printf("renderer could not be created : %s", SDL_GetError());

			} else {
                printf("renderer created successfully\n");
				SDL_RenderSetLogicalSize(renderer, BOARD_WIDTH_PX, BOARD_HEIGHT_PX);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_Image could not init : %s", IMG_GetError());
					success = false;
				} else {
                    printf("SDL_Image initialized successfully\n");
					if (TTF_Init() == -1) {
                        printf("SDL_TTF failed to initialize: %s\n", TTF_GetError());
                    } else {
                        printf("SDL_TTF initialized successfully\n");
                    }
				}
			}
		}
	}
	return success;
}

bool handleEvents() {
    SDL_Event e;
    bool quit = false;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }

        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }
    return quit;
}

void render(Board *board) {
    SDL_SetRenderDrawColor(renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(renderer);

    // board->hud->draw();
    board->drawGrid(renderer);
    for (Tetromino *tetra : board->tetrominos) {
        tetra->draw();
    }

    SDL_RenderPresent(renderer);
}

void gameLoop() {
    Timer fpsTimer;
    Board *board = new Board(renderer);
    board->addTetromino(new Tetromino(renderer, T, 0, 0));

    bool quit = false;
    uint32_t countedFrames = 0;
    float fps = 0.f;
    
    fpsTimer.start();

    while (!quit) {
        quit = handleEvents();

        render(board);

        fps = countedFrames / (fpsTimer.getTicks() / 1000.f);
        board->hud->update(fps);
        countedFrames++;
    }
    delete board;
}

void close() {		
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(void) {
    
    if (init()) {
        gameLoop();
    }
    close();

    return 0;
}
