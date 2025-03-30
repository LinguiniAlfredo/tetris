#include "board.h"
#include "utils/vec2.h"

using namespace std;

Board::Board(SDL_Renderer *renderer) {
    this->renderer = renderer;
    this->hud = new HUD(renderer);
}

Board::Board(SDL_Renderer *renderer, double gravity) {
    this->renderer = renderer;
    this->hud = new HUD(renderer);
    this->gravity = gravity;
    addRandomTetromino();
}

Board::~Board(){
    delete hud;
    for (Tetromino* tetromino : tetrominos) {
        delete tetromino;
    }
}

void Board::addRandomTetromino() {
    int randomType = rand() % 5 + 2;
    addTetromino((TetrominoType)randomType, {3, 3});
}

void Board::handleEvent(const SDL_Event& e) {
    activeTetromino->handleEvent(e);
}

void Board::update() {
    activeTetromino->update();
    if (activeTetromino->position.y == height - activeTetromino->texture->height) {
        if (lockFrameCount == lockFrames) {
            activeTetromino = nullptr;
            addRandomTetromino();
            lockFrameCount = 0;
        }
        lockFrameCount++;
    }
}

void Board::drawTetrominos() {
    for (Tetromino *tetra : tetrominos) {
        tetra->draw();
    }
}

void Board::drawGrid() {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 240);

    for (int x = 0; x < width; x = x+8) {
        SDL_RenderDrawLine(renderer, x, 0, x, height);
        for (int y = 0; y < height; y = y+8) {
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }
    }
}

void Board::addTetromino(TetrominoType type, Vec2 position) {
    Tetromino *tetromino = new Tetromino(renderer, this, type, position);
    this->activeTetromino = tetromino;
    
    tetrominos.push_back(tetromino);
}

