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
    int widthPx = 10 * 8;
    int heightPx = 20 * 8;

    void drawGrid(SDL_Renderer* renderer);
    void addTetromino(Tetromino*);
};