#include "board.h"
#include "utils/vec2.h"
#include "components/collision.h"
#include <algorithm>

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
    addTetromino((TetrominoType)randomType, bagPiece);
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

void Board::checkLineClear() {
    int maxY = 0;
    int numBlocks;
    int linesCleared = 0;

    for (int y = (height / 8) - 1; y >= 0; y--) {
        numBlocks = 0;
        for (int x = 0; x < width / 8; x++) {
            if (containsBlock(x, y)) {
                numBlocks++;
            }
        }
        if (numBlocks == width / 8) {
            maxY = y;
            clearLine(y);
            linesCleared++;
        }
    }
    movePiecesDown(maxY, linesCleared);
}

bool Board::containsBlock(int x, int y) {
    for (Tetromino *piece : tetrominos) {
        for (auto const& [block, blockPos] : piece->textures) {
            if (piece->position.x / 8 + blockPos.x / 8 == x &&
                piece->position.y / 8 + blockPos.y / 8 == y) {
                return true;
            }
        }
    }
    return false;
}

void Board::clearLine(int y) {
    for (Tetromino *piece : tetrominos) {
        for (auto [block, blockPos] : piece->textures) {
            if (piece->position.y / 8 + blockPos.y / 8 == y) {
                piece->textureTrash.push_back(block);
            }
        }
        for (auto [collider, pos] : piece->colliders) {
            if (piece->position.y / 8 + pos.y / 8 == y) {
                piece->colliderTrash.push_back(collider);
            }
        }
        if (piece->textures.size() == 0) {
            tetrominoTrash.push_back(piece);
        }
    }
    removeTrash();
}

void Board::movePiecesDown(int y, int linesCleared) {
    for (Tetromino *piece : tetrominos) {
        for (auto [block, pos] : piece->textures) {
            if (pos.y < y) {
                // TODO - refactor drop() to be able to use this here instead
                for(auto const& [collider, pos] : piece->colliders) {
                    collider->box->y += 8 * linesCleared;
                }

                if (piece->inBounds() && !piece->checkCollisions()) {
                    piece->position.y += 8 * linesCleared;

                } else {
                    for (auto const& [collider, pos] : piece->colliders) {
                        collider->box->y -= 8 * linesCleared;
                    }
                }

            }
        }
    }
}

void Board::removeTrash() {
    for (Tetromino *piece : tetrominos) {
        for (Texture *block : piece->textureTrash) {
            piece->textures.erase(block);
            delete block;
        }
        for (Collision *collider : piece->colliderTrash) {
            piece->colliders.erase(collider);
            delete collider;
        }
    piece->colliderTrash.clear();
    piece->textureTrash.clear();
    }


    for (Tetromino *trashPiece : tetrominoTrash) {
        tetrominos.erase(remove(tetrominos.begin(), tetrominos.end(), trashPiece), tetrominos.end());
        delete trashPiece;
    }
    tetrominoTrash.clear();
}
