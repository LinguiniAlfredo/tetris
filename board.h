#pragma once

#include "SDL2/SDL.h"
#include "ui/hud.h"
#include "entities/tetromino.h"
#include <vector>

class Board {
public:
    Board(SDL_Renderer* renderer);
    ~Board();

    HUD* hud;
    vector<Tetromino*> tetrominos;

    Tetromino *activeTetromino = nullptr;

    // 1G = 1 cell per frame, 0.1G = 1 cell per 10 frames
    int gravity = 2;

    int width = 10 * 8;
    int height = 20 * 8;

    void handleEvent(const SDL_Event& e);
    void update();

    void addTetromino(Tetromino *tetromino);
    void drawGrid(SDL_Renderer *renderer);
    void drawTetrominos(SDL_Renderer *renderer);
    
};
