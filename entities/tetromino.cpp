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
    
    acquireTetrominoTextures(type, renderer);
    generateColliders(renderer);
}

Tetromino::~Tetromino() {
    for (auto const& [texture, pos] : textures) {
        delete texture;
    }
    for (int i = 0; i < colliders.size(); i++) {
        delete colliders.at(i);
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
    for (Collision *thisCollider : colliders) {
        for (Tetromino *piece : board->tetrominos) {
            if (piece != this) {
                for (Collision *collider : piece->colliders) {
                    if (SDL_HasIntersection(thisCollider->box, collider->box)) {
                        colliding = true;
                        break;
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
    for (Collision *collider : colliders) {
        collider->box->x += velocity.x;
    }
    if (inBounds() && !checkCollisions()) {
        position.x += velocity.x;
    } else {
        for (Collision *collider : colliders) {
            collider->box->x -= velocity.x;
        }
    }
    velocity.x = 0;
}

void Tetromino::drop() {
    trueYPos += board->gravity;
    // TODO - move collider by true position to check faster
    if (floor(trueYPos) == (float)trueYPos) {

        for(Collision *collider : colliders) {
            collider->box->y += 8;
        }

        if (inBounds() && !checkCollisions()) {
            position.y += 8;
        } else {
            for (Collision *collider : colliders) {
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
    for (Collision *collider : colliders) {
        if (collider->box->x < 0 || 
            collider->box->y < 0 || 
            collider->box->x > board->width - collider->box->w ||
            collider->box->y > board->height - collider->box->h) {
            return false;
        }
    }
    return true;
}

void Tetromino::acquireTetrominoTextures(TetrominoType type, 
        SDL_Renderer *renderer) {

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
            constructLTexture(renderer, "resources/textures/L_single.png");
            break;
    }
}

void Tetromino::constructLTexture(SDL_Renderer *renderer, string filepath) {
   for (int i = 0; i < 4; i++) {
       Vec2 relPosition = { i, 0 };
       textures.insert({new Texture(renderer, filepath, 0, 0), relPosition});
   }
}

void Tetromino::generateColliders(SDL_Renderer *renderer) {
    for (auto const& [texture, pos] : textures) {
        colliders.push_back(new Collision(renderer, pos.x, pos.y, 
            texture->width, texture->height));
    }
}
