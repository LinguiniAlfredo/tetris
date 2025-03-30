#include "board.h"

Board::Board(SDL_Renderer *renderer) {
    hud = new HUD(renderer);
}

Board::~Board(){
    delete hud;
    for (Tetromino* tetromino : tetrominos) {
        delete tetromino;
    }
}

void Board::handleEvent(const SDL_Event& e) {
    activeTetromino->handleEvent(e);
}

void Board::update() {
    activeTetromino->update();
}

void Board::drawTetrominos(SDL_Renderer *renderer) {
    for (Tetromino *tetra : tetrominos) {
        tetra->draw();
    }
}

void Board::drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 240);

    for (int x = 0; x < width; x = x+8) {
        SDL_RenderDrawLine(renderer, x, 0, x, height);
        for (int y = 0; y < height; y = y+8) {
            SDL_RenderDrawLine(renderer, 0, y, width, y);
        }
    }
}

void Board::addTetromino(Tetromino *tetromino) {
    tetrominos.push_back(tetromino);
    this->activeTetromino = tetromino;
    tetromino->board = this;
}

