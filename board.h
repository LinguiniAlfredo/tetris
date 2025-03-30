#pragma once

#include "SDL2/SDL.h"
#include "ui/hud.h"
#include "entities/tetromino.h"
#include <vector>

class Board {
public:
    Board(SDL_Renderer *renderer, double gravity);
    ~Board();

    HUD* hud;
    SDL_Renderer* renderer;
    vector<Tetromino*> tetrominos;

    Tetromino *activeTetromino = nullptr;

    bool softDrop = false;
    double initialGravity;
    double gravity = 0.016666667;    // 1 cell per 60 frames
    double softDropGravity = 0.5;
    double hardDropGravity = 1.0;

    int width = 10 * 8;
    int height = 20 * 8;

    int lockFrames = 30;
    int lockFrameCount = 0;

    void handleEvent(const SDL_Event& e);
    void update();

    void drawGrid();
    void drawTetrominos();
    
private:
    void addRandomTetromino();
    void addTetromino(TetrominoType type, Vec2 position);
};
