#pragma once

#include "SDL2/SDL.h"
#include "ui/hud.h"
#include "entities/tetromino.h"
#include <vector>
#include "components/animation.h"

extern GameState gameState;

class Board {
public:
    Board(SDL_Renderer *renderer, double gravity);
    ~Board();

    HUD* hud;
    SDL_Renderer* renderer;
    vector<Tetromino*> tetrominos;
    vector<Tetromino*> tetrominoTrash;

    Tetromino *activeTetromino = nullptr;
    Tetromino *nextTetromino = nullptr;

    vector<Animation*> animations;

    Texture *sidebar = nullptr;

    bool softDrop = false;
    bool hardDrop = false;
    double initialGravity;
    double gravity = 0.016666667;    // 1 cell per 60 frames
    double softDropGravity = 0.5;

    int width = 10 * 8;
    int height = 20 * 8;

    int lockFrames = 30;
    int lockFrameCount = 0;
    int linesCleared = 0;
    bool collapseStart = false;
    int collapseFrames = 6;
    int collapseFrameCount = 0;

    Vec2 sidebarPosition = { 10, 0 };
    Vec2 spawnPosition = { 3, 1 };
    Vec2 bagPosition = { 10, 2 };

    double score = 0;
 
    void handleEvent(const SDL_Event& e);
    void update(int currentFrame);
    void draw();
    void drawSidebar();
    void drawAnimations();
    void drawGrid();
    void drawTetrominos();
    void drawHud();
    void cycleTetrominos();
    void prepTetrominos();
    void prepAnimations();
    void addRandomTetromino(bool bagPiece);
    void addTetromino(TetrominoType type, bool bagPiece);
    void checkLineClear();
    bool containsBlock(int x, int y);
    void clearLine(int y);
    void movePiecesDown();
    void moveRowDown(int row, int numLines);
    void removeClearedPieces();
    void adjustBagPosition(Tetromino *tetromino, TetrominoType type);
    bool checkForGameOver();

    bool animationsPlaying();

};
