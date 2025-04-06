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
    this->ghostPosition = position;
    this->colliderPosition = position;
    this->trueYPos = (float) position.y;
    
    constructTetromino(type, renderer);
    generateColliders(renderer);
}

Tetromino::~Tetromino() {
    for (auto [texture, pos] : textures) {
        delete texture;
    }
    for (auto [collider, pos] : colliders) {
        delete collider;
    }
    for (auto [texture, pos] : ghostTextures) {
        delete texture;
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
                softDropStart = true;
                board->softDrop = true;
                break;
            case SDLK_d:
                velocity.x += 8;
                break;
            case SDLK_f:
                rotate(90);
                break;
            case SDLK_SPACE:
                hardDrop();
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
    Vec2 initialPosition = position;

    findGhostPosition();

    moveX();
    drop();

    checkLock(initialPosition);
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
    if (instaLock) {
        board->checkLineClear();
        board->cycleTetrominos();
        board->lockFrameCount = 0;
    }
    if (position.y == initialPosition.y) {
        if (board->lockFrameCount == board->lockFrames + 30) {
            board->checkLineClear();
            board->cycleTetrominos();
            board->lockFrameCount = 0;
        } else {
            board->lockFrameCount++;
        }
    } else {
        board->lockFrameCount = 0;
    }
}

void Tetromino::draw() const {
    if (textures.size() > 0) {
        for (auto const& [texture, pos] : textures) {
            texture->render(position.x + pos.x, position.y + pos.y);
        }
    }
    if (board->activeTetromino == this) {
        for (auto const& [texture, pos] : ghostTextures) {
            texture->render(ghostPosition.x + pos.x, ghostPosition.y + pos.y);
        }
    }
}

void Tetromino::rotate(int angle) {
    // rotx = radiusX*cos(radians) - radiusY*sin(radians)
    // roty = radiusX*sin(radians) + radiusY*cos(radians)

    double rad = angle * (M_PI/180);
    double c = cos(rad);
    double s = sin(rad);

    for (auto& [texture, pos] : textures) {
        double radX = pos.x - center.x;
        double radY = pos.y - center.y;
        pos.x = round(radX * c - radY * s + center.x);
        pos.y = round(radX * s + radY * c + center.y);
    }
    for (auto& [texture, pos] : ghostTextures) {
        double radX = pos.x - center.x;
        double radY = pos.y - center.y;
        pos.x = round(radX * c - radY * s + center.x);
        pos.y = round(radX * s + radY * c + center.y);
    }

    for (auto& [collider, pos] : colliders) {
        double radX = pos.x - center.x;
        double radY = pos.y - center.y;
        pos.x = round(radX * c - radY * s + center.x);
        pos.y = round(radX * s + radY * c + center.y);

        collider->box->x = colliderPosition.x + pos.x;
        collider->box->y = colliderPosition.y + pos.y;
    }
}

void Tetromino::moveX() {
    for (auto const& [collider, pos]: colliders) {
        collider->box->x += velocity.x;
    }
    colliderPosition.x += velocity.x;

    if (inBounds() && !checkCollisions()) {
        position.x += velocity.x;
    } else {
        for (auto const& [collider, pos]: colliders) {
            collider->box->x -= velocity.x;
        }
        colliderPosition.x -= velocity.x;
    }
    velocity.x = 0;
}

void Tetromino::drop() {

    if (softDropStart) {
        trueYPos = ceil(trueYPos);
    } else {
        trueYPos += board->gravity;
    }

    if (floor(trueYPos) == (float)trueYPos) {
        for(auto const& [collider, pos] : colliders) {
            collider->box->y += 8;
        }
        colliderPosition.y += 8;

        if (inBounds() && !checkCollisions()) {
            position.y += 8;
        } else {
            for (auto const& [collider, pos] : colliders) {
                collider->box->y -= 8;
            }
            colliderPosition.y -= 8;
        }

        if (board->softDrop) {
            board->gravity = board->softDropGravity;
        } else {
            board->gravity = board->initialGravity;
        }
        softDropStart = false;
    }
}

void Tetromino::hardDrop() {
    int distance = ghostPosition.y - position.y;
    position.y = ghostPosition.y;
    for (auto [collider, pos] : colliders) {
        collider->box->y += distance;
    }
    colliderPosition.y += distance;
    instaLock = true;
}

// TODO - move this function into checkCollisions
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

void Tetromino::findGhostPosition() {
    int numBlocksMoved = 0;
    ghostPosition.y = position.y;

    while (inBounds() && !checkCollisions()) {
        for(auto [collider, pos] : colliders) {
            collider->box->y += 8;
        }
        numBlocksMoved++;
    }

    for (auto [collider, pos] : colliders) {
        collider->box->y -= 8 * numBlocksMoved;
    }
    numBlocksMoved--;

    ghostPosition.y += 8 * numBlocksMoved;
    ghostPosition.x = position.x;
}

// TODO - is there a prettier way?
void Tetromino::constructTetromino(TetrominoType type, 
        SDL_Renderer *renderer) {

    string filepath;

    switch(type) {
        case I:
            filepath = "resources/textures/I";
            for (int i = 0; i < 4; i++) {
                textures.insert( {new Texture(renderer, filepath + "_single.png", 0, 0), { i, 0 } });
                ghostTextures.insert( {new Texture(renderer, filepath + "_ghost.png", 0, 0), { i, 0 } });
            }
            center = { 1.5, 0.5 };
            break;
        case O:
            filepath = "resources/textures/O";
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0, 0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1, 0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0, 1 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1, 1 } });

            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0, 0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1, 0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0, 1 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1, 1 } });
            
            center = { 0.5, 0.5 };
            break;
        case T:
            filepath = "resources/textures/T";
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 2,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1, -1 } });

            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 2,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1, -1 } });

            center = { 1.0, 0.0 };
            break;
        case S:
            filepath = "resources/textures/S";
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1, -1 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 2, -1 } });

            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1, -1 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 2, -1 } });

            center = { 1.0, 0.0 };
            break;
        case Z:
            filepath = "resources/textures/Z";
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1,  1 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 2,  1 } });

            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1,  1 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 2,  1 } });

            center = { 1.0, 1.0 };
            break;
        case J:
            filepath = "resources/textures/J";
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0,  1 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1,  1 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 2,  1 } });

            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0,  1 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1,  1 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 2,  1 } });

            center = { 1.0, 1.0 };
            break;
        case L:
            filepath = "resources/textures/L";
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 0,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 1,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 2,  0 } });
            textures.insert({ new Texture(renderer, filepath + "_single.png", 0, 0), { 2, -1 } });

            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 0,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 1,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 2,  0 } });
            ghostTextures.insert({ new Texture(renderer, filepath + "_ghost.png", 0, 0), { 2, -1 } });

            center = { 1.0, 0.0 };
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
