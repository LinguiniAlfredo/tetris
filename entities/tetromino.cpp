#include "tetromino.h"
#include "../board.h"
#include "../components/collision.h"
#include <cmath>

using namespace std;

Tetromino::Tetromino(SDL_Renderer *renderer, Board *board, 
        TetrominoType type, Vec2 position) {

    this->board = board;
    this->type = type;
    this->position = position;
    this->trueYPos = (float) position.y;
    
    constructTetromino(type, renderer);
    generateColliders(renderer);
}

Tetromino::~Tetromino() {
    for (auto const& [texture, pos] : textures) {
        delete texture;
    }
    for (auto const& [collider, pos] : colliders) {
        delete collider;
    }
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
    for (auto const& [thisCollider, thisPos]: colliders) {
        for (Tetromino *piece : board->tetrominos) {
            if (piece != this) {
                for (auto const& [thatCollider, thatpos]: piece->colliders) {
                    if (SDL_HasIntersection(thisCollider->box, thatCollider->box)) {
                        colliding = true;
                        return colliding;
                    } else {
                        colliding = false;
                    }
                }
            }

        }
    }
    return colliding;
}

void Tetromino::checkLock(Vec2 initialPosition) {
    if (position.y == initialPosition.y) {
        // TODO - check trueposition to be quicker
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
    if (textures.size() > 0) {
        for (auto const& [texture,pos] : textures) {
            texture->render(position.x + pos.x, position.y + pos.y);
        }
    }
}

void Tetromino::rotate() {

}

void Tetromino::moveX() {
    for (auto const& [collider, pos]: colliders) {
        collider->box->x += velocity.x;
    }
    if (inBounds() && !checkCollisions()) {
        position.x += velocity.x;
    } else {
        for (auto const& [collider, pos]: colliders) {
            collider->box->x -= velocity.x;
        }
    }
    velocity.x = 0;
}

void Tetromino::drop() {
    trueYPos += board->gravity;
    // TODO - move collider by true position to check faster
    if (floor(trueYPos) == (float)trueYPos) {

        for(auto const& [collider, pos] : colliders) {
            collider->box->y += 8;
        }

        if (inBounds() && !checkCollisions()) {
            position.y += 8;
        } else {
            for (auto const& [collider, pos] : colliders) {
                collider->box->y -= 8;
            }
        }

        if (board->softDrop) {
            board->gravity = board->softDropGravity;
        } else {
            board->gravity = board->initialGravity;
        }
    }
}

bool Tetromino::inBounds() {
    for (auto const& [collider, pos] : colliders) {
        if (collider->box->x < 0 || 
            collider->box->y < 0 || 
            collider->box->x > board->width - collider->box->w ||
            collider->box->y > board->height - collider->box->h) {
            return false;
        }
    }
    return true;
}

void Tetromino::constructTetromino(TetrominoType type, 
        SDL_Renderer *renderer) {

    string filepath;

    switch(type) {
        case I:
            filepath = "resources/textures/I_single.png";
            for (int i = 0; i < 4; i++) {
                textures.insert( {new Texture(renderer, filepath, 0, 0), { i, 0 } });
            }
            break;
        case O:
            filepath = "resources/textures/O_single.png";
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0, 0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1, 0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0, 1 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1, 1 } });
            break;
        case T:
            filepath = "resources/textures/T_single.png";
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 2,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1, -1 } });
            break;
        case S:
            filepath = "resources/textures/S_single.png";
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1, -1 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 2, -1 } });
            break;
        case Z:
            filepath = "resources/textures/Z_single.png";
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1,  1 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 2,  1 } });
            break;
        case J:
            filepath = "resources/textures/J_single.png";
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0,  1 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1,  1 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 2,  1 } });
            break;
        case L:
            filepath = "resources/textures/L_single.png";
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 2,  0 } });
            textures.insert({ new Texture(renderer, filepath, 0, 0), { 2, -1 } });
            break;
    }
}

void Tetromino::generateColliders(SDL_Renderer *renderer) {
    for (auto const& [texture, pos] : textures) {
        colliders.insert({new Collision(renderer, 
                    position.x + pos.x, position.y + pos.y, 
                    texture->width, texture->height), pos});
    }
}
