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
                // TODO - cant snap to bottom, will clip through stacked pieces
                //position.y = board->height - texture->height;
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

void Tetromino::update() {
    moveX();
    drop();
}

void Tetromino::moveX() {
    position.x += velocity.x;
    if (!inBounds() || checkCollisions()) {
        position.x -= velocity.x;
    }
    collider->box->x = position.x;
    velocity.x = 0;
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
    // TODO - use an epsilon for this instead of casting
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
    collider->box->y = position.y;
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
