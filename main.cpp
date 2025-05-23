#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "gamestate.h"
#include "board.h"
#include "ui/hud.h"
#include "utils/timer.h"
#include "entities/tetromino.h"
#include "components/collision.h"
#include "components/animation.h"

using namespace std;

// TODO - wall / floor bounce
//      - stash piece
//      - main menu
//      - scoring
//      - lock effect
//      - hard drop effect
//      - music/sound

GameState gameState;

const float TICKS_PER_FRAME = 1000.f / 60;

const int BOARD_WIDTH_PX = 16 * 8;
const int BOARD_HEIGHT_PX = 20 * 8;
const int SCREEN_WIDTH = BOARD_WIDTH_PX * 4;
const int SCREEN_HEIGHT = BOARD_HEIGHT_PX * 4;


SDL_Renderer *renderer = nullptr;
SDL_Window *window = nullptr;
Board *currentBoard = nullptr;
Texture *gameOverTexture = nullptr;

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
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

void changeLevel(int level) {
    switch(level) {
        case 0:  // menu
            break;
        case 1:
            delete currentBoard;
            currentBoard = new Board(renderer, 0.016666667);
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}

void toggleDebug() {
    gameState.debug = !gameState.debug;
}

void renderColliders() {

   for (Tetromino *piece : currentBoard->tetrominos) {
       if (piece->colliding) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
       } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
       }
       for (auto const& [collider, pos]: piece->colliders) {
            SDL_Rect *correctedBox = new SDL_Rect {
                piece->colliderPosition.x + pos.x, 
                piece->colliderPosition.y + pos.y, 
                collider->box->w, 
                collider->box->h
            };
        
            SDL_RenderDrawRect(renderer, correctedBox);
            delete correctedBox;
       }
   }
}

void renderGameOverScreen() {
    if (gameOverTexture == nullptr) {
        gameOverTexture = new Texture(renderer, "resources/textures/gameover.png", 0, 0);
    }
    gameOverTexture->render(0, 0);
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
                case SDLK_1:
                    changeLevel(1);
                    break;
                case SDLK_F1:
                    toggleDebug();
                    break;
                default:
                    break;
            }
        }

        currentBoard->handleEvent(e);

        if (gameState.gameover) {
            if (e.type == SDL_KEYDOWN) {
                gameState.gameover = false;
                changeLevel(1);
            }
        }
    }
    return quit;
}


void update(int currentFrame) {
    currentBoard->update(currentFrame);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0xF5, 0xF5, 0xF5, 0xFF);
    SDL_RenderClear(renderer);

    currentBoard->draw();
    
    if (gameState.debug) {
        renderColliders();
    }
    if (gameState.gameover) {
        renderGameOverScreen();
    }

    SDL_RenderPresent(renderer);
}

void gameLoop() {
    Timer totalTimer;
    Timer deltaTimer;
    Timer capTimer;

    changeLevel(1);

    bool quit = false;
    uint32_t countedFrames = 0;
    float fps = 0.f;
    //float deltaTime = 0.f;
    
    totalTimer.start();
    deltaTimer.start();

    while (!quit) {
        deltaTimer.start();
        capTimer.start();

        quit = handleEvents();

        if (!gameState.gameover) {
            update(countedFrames);
        }
        render();

        fps = countedFrames / (totalTimer.getTicks() / 1000.f);
        currentBoard->hud->update(currentBoard->score, fps);

        countedFrames++;
        int ticks = capTimer.getTicks();
        if (ticks < TICKS_PER_FRAME) {
            SDL_Delay(TICKS_PER_FRAME - ticks);
        }
    }
    delete currentBoard;
}

void close() {		
    delete gameOverTexture;
    gameOverTexture = nullptr;

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
