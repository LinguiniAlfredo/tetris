#include "board.h"
#include "utils/vec2.h"
#include "components/collision.h"

using namespace std;

Board::Board(SDL_Renderer *renderer, double gravity) {
    this->renderer = renderer;
    this->hud = new HUD(renderer);
    this->initialGravity = gravity;
    this->gravity = gravity;
    prepTetrominos();
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

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect *gridBorder = new SDL_Rect{ -1, 0, width + 2, height + 2 };
    SDL_RenderDrawRect(renderer, gridBorder);
    delete gridBorder;
}

void Board::prepTetrominos() {
    addRandomTetromino(false);
    addRandomTetromino(true);
}

void Board::addRandomTetromino(bool bagPiece) {
    // TODO - add regulation "bag" system
    int randomType = rand() % 7 + 0;
    //addTetromino((TetrominoType)randomType, bagPiece);
    addTetromino(I, bagPiece);
}

void Board::cycleTetrominos() {
    nextTetromino->position = spawnPosition;
    for (auto const& [collider, relPos] : nextTetromino->colliders) {
        collider->box->x = spawnPosition.x + relPos.x;
        collider->box->y = spawnPosition.y + relPos.y;
    }
    activeTetromino = nextTetromino;
    addRandomTetromino(true);
}

void Board::addTetromino(TetrominoType type, bool bagPiece) {
    Vec2 position = bagPiece ? bagPosition : spawnPosition;
    Tetromino *tetromino = new Tetromino(renderer, this, type, position);
    
    if (bagPiece) {
        this->nextTetromino = tetromino;
    } else {
        this->activeTetromino = tetromino;
    }

    tetrominos.push_back(tetromino);
}

