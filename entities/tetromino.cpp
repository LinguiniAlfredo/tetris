#include "tetromino.h"
#include "../board.h"
#include <cmath>

using namespace std;

Tetromino::Tetromino(SDL_Renderer *renderer, Board *board, 
        TetrominoType type, Vec2 position) {

    this->board = board;
    this->type = type;
    this->texture = acquireTetrominoTexture(type, renderer);
    this->position = position;
    this->trueYPos = (float) position.y;
}

Tetromino::~Tetromino() {
    if (texture != nullptr) {
        delete texture;
        texture = nullptr;
    }
}

void Tetromino::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
                position.x -= 8;
                if (!inBounds()) {
                    position.x += 8;
                }
                break;
            case SDLK_s:
                // soft drop (increases gravity)
                board->softDrop = true;
                break;
            case SDLK_d:
                position.x += 8;
                if (!inBounds()) {
                    position.x -= 8;
                }
                break;
            case SDLK_SPACE:
                // hard drop (snaps to bottom)
                position.y = board->height - texture->height;
                break;
            default:
                break;
        }
    }
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
            case SDLK_s:
                board->softDrop = false;
                break;
        }
    }
}

void Tetromino::update() {
    drop();
}

void Tetromino::draw() const {
    if (texture != nullptr) {
        texture->render(position.x, position.y);
    }
}

void Tetromino::rotate() {

}

void Tetromino::drop() {
    trueYPos += board->gravity;
    if (floor(trueYPos) == (float)trueYPos) {
        position.y += 8;

        if (board->softDrop) {
            board->gravity = board->softDropGravity;
        } else {
            board->gravity = board->initialGravity;
        }

        if (!inBounds()) {
            position.y -= 8;
        }
    }
}

bool Tetromino::inBounds() {
    return position.x >= 0 && 
           position.y >= 0 && 
           position.x <= board->width - texture->width && 
           position.y <= board->height - texture->height;
}

Texture* Tetromino::acquireTetrominoTexture(TetrominoType type, SDL_Renderer *renderer) {
    string filepath;
    switch(type) {
        case I:
            break;
        case O:
            break;
        case T:
            filepath = "resources/textures/T.png";
            break;
        case S:
            filepath = "resources/textures/S.png";
            break;
        case Z:
            filepath = "resources/textures/Z.png";
            break;
        case J:
            filepath = "resources/textures/J.png";
            break;
        case L:
            filepath = "resources/textures/L.png";
            break;
        
    }
    return new Texture(renderer, filepath, 0, 0);


}
