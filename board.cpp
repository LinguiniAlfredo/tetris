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

void Board::drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 240, 2, 240, 240);

    for (int x = 0; x < widthPx; x = x+8) {
        SDL_RenderDrawLine(renderer, x, 0, x, heightPx);
        for (int y = 0; y < heightPx; y = y+8) {
            SDL_RenderDrawLine(renderer, 0, y, widthPx, y);
        }
    }
}

void Board::addTetromino(Tetromino* tetromino) {
    tetrominos.push_back(tetromino);
}