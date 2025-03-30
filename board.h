#pragma once

#include "SDL2/SDL.h"
#include "ui/hud.h"
#include "entities/tetromino.h"
#include <vector>

class Board {
public:
    Board(SDL_Renderer *renderer);
    Board(SDL_Renderer *renderer, double gravity);
    ~Board();

    HUD* hud;
    SDL_Renderer* renderer;
    vector<Tetromino*> tetrominos;

    Tetromino *activeTetromino = nullptr;

    double gravity = 0.016666667;    // 1 cell per 60 frames
    //double gravity = 0.05;           // 1 cell per 20 frames 
    //double gravity = 1.0;            // 1 cell per frame

    int width = 10 * 8;
    int height = 20 * 8;

    void handleEvent(const SDL_Event& e);
    void update();

    void drawGrid();
    void drawTetrominos();
    
private:
    void addRandomTetromino();
    void addTetromino(TetrominoType type, Vec2 position);
};
