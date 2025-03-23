#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "ui/hud.h"
#include "utils/timer.h"
#include "entities/player.h"

const int SCREEN_WIDTH = 1920 / 2;
const int SCREEN_HEIGHT = 1080 / 2;
const int INTERNAL_SCREEN_WIDTH = 320;
const int INTERNAL_SCREEN_HEIGHT = 180;
const int CENTER_X = INTERNAL_SCREEN_WIDTH / 2;
const int CENTER_Y = INTERNAL_SCREEN_HEIGHT / 2;
const int NUM_TILES_WIDE = INTERNAL_SCREEN_WIDTH / 8;
const int NUM_TILES_HIGH = INTERNAL_SCREEN_HEIGHT / 8;

SDL_Renderer *renderer = nullptr;
SDL_Window *window = nullptr;

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	} else {
		printf("SDL initialized successfully\n");
		window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
				SDL_RenderSetLogicalSize(renderer, INTERNAL_SCREEN_WIDTH, INTERNAL_SCREEN_HEIGHT);

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

void render(const Player &player, const HUD &hud) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        player.draw();
        hud.draw();

        SDL_RenderPresent(renderer);
}

void gameLoop() {
    Timer fpsTimer;
    Player player = Player(renderer);
    HUD hud = HUD(renderer);

    bool quit = false;
    uint32_t countedFrames = 0;
    float fps = 0.f;
    
    fpsTimer.start();

    while (!quit) {
        quit = handleEvents();

        render(player, hud);

        fps = countedFrames / (fpsTimer.getTicks() / 1000.f);
        hud.update(fps);
        countedFrames++;
    }
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