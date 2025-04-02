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
    Tetromino *nextTetromino = nullptr;

    bool softDrop = false;
    double initialGravity;
    double gravity = 0.016666667;    // 1 cell per 60 frames
    double softDropGravity = 0.5;
    double hardDropGravity = 1.0;

    int width = 10 * 8;
    int height = 20 * 8;

    int lockFrames = 30;
    int lockFrameCount = 0;

    Vec2 spawnPosition = { 3, 3 };
    Vec2 bagPosition = { 11, 1 };
 
    void handleEvent(const SDL_Event& e);
    void update();
    void drawGrid();
    void drawTetrominos();
    void cycleTetrominos();
    void prepTetrominos();
    void addRandomTetromino(bool bagPiece);
    void addTetromino(TetrominoType type, bool bagPiece);
};
