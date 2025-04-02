#include "tetromino.h"
#include "../board.h"
#include "../components/collision.h"
#include <cmath>

using namespace std;

Tetromino::Tetromino(SDL_Renderer *renderer, Board *board, 
        TetrominoType type, Vec2 position) {

    this->board = board;
    this->type = type;
    this->texture = acquireTetrominoTexture(type, renderer);
    this->position = position;
    this->trueYPos = (float) position.y;
    this->collider = new Collision(renderer, position.x, position.y, 
            texture->width, texture->height);
}

Tetromino::~Tetromino() {
    delete texture;
    delete collider;
}

void Tetromino::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_a:
                velocity.x -= 8;
                break;
            case SDLK_s:
                // soft drop (increases gravity)
                board->softDrop = true;
                break;
            case SDLK_d:
                velocity.x += 8;
                break;
            case SDLK_SPACE:
                // hard drop (snaps to bottom)
                // TODO - increase gravity like softDrop
                // but with no latency
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

bool Tetromino::checkCollisions() {
    for (Tetromino *piece : board->tetrominos) {
        if (piece != this) {
            if (SDL_HasIntersection(collider->box, piece->collider->box)) {
                colliding = true;
                break;
            } else {
                colliding = false;
            }
        }
    }
    return colliding;
}

void Tetromino::checkLock(Vec2 initialPosition) {
    if (position.y == initialPosition.y) {
        if (board->lockFrameCount == board->lockFrames + 60) {
            board->cycleTetrominos();
            board->lockFrameCount = 0;
        } else {
            board->lockFrameCount++;
        }
    } else {
        board->lockFrameCount = 0;
    }
}

void Tetromino::update() {
    Vec2 initialPosition = position;

    moveX();
    drop();
    checkLock(initialPosition);
}

void Tetromino::draw() const {
    if (texture != nullptr) {
        texture->render(position.x, position.y);
    }
}

void Tetromino::rotate() {

}

void Tetromino::moveX() {
    collider->box->x += velocity.x;
    if (inBounds() && !checkCollisions()) {
        position.x += velocity.x;
    } else {
        collider->box->x -= velocity.x;
    }
    velocity.x = 0;
}

void Tetromino::drop() {
    trueYPos += board->gravity;
    // TODO - use an epsilon for this instead of casting
    if (floor(trueYPos) == (float)trueYPos) {
        collider->box->y += 8;
        if (inBounds() && !checkCollisions()) {
            position.y += 8;
        } else {
            collider->box->y -= 8;
        }

        if (board->softDrop) {
            board->gravity = board->softDropGravity;
        } else {
            board->gravity = board->initialGravity;
        }
    }
}

bool Tetromino::inBounds() {
    return collider->box->x >= 0 && 
           collider->box->y >= 0 && 
           collider->box->x <= board->width - collider->box->w && 
           collider->box->y <= board->height - collider->box->h;

}

Texture* Tetromino::acquireTetrominoTexture(TetrominoType type, SDL_Renderer *renderer) {
    string filepath;
    switch(type) {
        case I:
            filepath = "resources/textures/I.png";
            break;
        case O:
            filepath = "resources/textures/O.png";
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
