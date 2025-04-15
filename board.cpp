#include "board.h"
#include "utils/vec2.h"
#include "components/collision.h"
#include <algorithm>

using namespace std;

Board::Board(SDL_Renderer *renderer, double gravity) {
    this->renderer = renderer;
    this->hud = new HUD(renderer);
    this->sidebar = new Texture(renderer, "resources/textures/sidebar.png", 0, 0);
    this->initialGravity = gravity;
    this->gravity = gravity;
    prepTetrominos();
    prepAnimations();
}

Board::~Board(){
    delete hud;
    delete sidebar;

    for (Tetromino* tetromino : tetrominos) {
        delete tetromino;
    }
    for (Animation* animation : animations) {
        delete animation;
    }
}

void Board::handleEvent(const SDL_Event& e) {
    activeTetromino->handleEvent(e);
}

void Board::update(int currentFrame) {
    for (Animation* animation : animations) {
        if(animation->playing) {
            animation->update(currentFrame);
        }
    }

    activeTetromino->update(currentFrame);

    if (activeTetromino->locked) {
        lockFrameCount = 0;
        checkLineClear();
        cycleTetrominos(); // TODO - do this after collapsing
    }

    if (linesCleared > 0 && !animationsPlaying()) {
        removeClearedPieces();
        linesCleared = 0;
        collapseStart = true;
    }
    if (collapseStart) {
        if (collapseFrameCount == collapseFrames) {
            movePiecesDown();
            collapseFrameCount = 0;
            collapseStart = false;
        }
        collapseFrameCount++;
    }
}

void Board::draw() {
    drawSidebar();
    drawTetrominos();
    drawGrid();
    drawAnimations();
    drawHud();
}

void Board::drawSidebar() {
    sidebar->render(sidebarPosition.x, sidebarPosition.y);
}

void Board::drawAnimations() {
    for (Animation* animation : animations) {
        if(animation->playing) {
            animation->draw();
        }
    }
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
            SDL_RenderDrawLine(renderer, 0, y, width - 1, y);
        }
    }

    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //SDL_RenderDrawLine(renderer, width, 0, width, height);
}

void Board::drawHud() {
    this->hud->draw();
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
    nextTetromino->colliderPosition = spawnPosition;
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
        adjustBagPosition(tetromino, type); // jank but whatevs
        this->nextTetromino = tetromino;
    } else {
        this->activeTetromino = tetromino;
    }

    tetrominos.push_back(tetromino);
}

void Board::adjustBagPosition(Tetromino *tetromino, TetrominoType type) {
    switch (type) {
        case O:
            tetromino->position.x += 16;
            tetromino->position.y -= 8;
            break;
        case T: 
        case S:
        case L: 
        case I:
            tetromino->position.x += 8;
            break;
        case J:
        case Z:
            tetromino->position.x += 8;
            tetromino->position.y -= 8;
            break;
    }
}

void Board::checkLineClear() {
    int numBlocks;

    for (int y = (height / 8) - 1; y >= 0; y--) {
        numBlocks = 0;
        for (int x = 0; x < width / 8; x++) {
            if (containsBlock(x, y)) {
                numBlocks++;
            }
        }
        if (numBlocks == width / 8) {
            clearLine(y);
            linesCleared++;
            score++;
        }
    }
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
                if(!(find(piece->textureTrash.begin(), piece->textureTrash.end(), block) != piece->textureTrash.end())){
                    piece->textureTrash.push_back(block);
                }
            }
        }
        for (auto [collider, pos] : piece->colliders) {
            if (piece->position.y / 8 + pos.y / 8 == y) {
                if(!(find(piece->colliderTrash.begin(), piece->colliderTrash.end(), collider) != piece->colliderTrash.end())){
                    piece->colliderTrash.push_back(collider);
                }
            }
        }
        if (piece->textures.size() == 0) {
            if(!(find(tetrominoTrash.begin(), tetrominoTrash.end(), piece) != tetrominoTrash.end())){
                tetrominoTrash.push_back(piece);
            }
        }
    }
    animations.at(y)->play();
}

void Board::movePiecesDown() {
    int emptyLines = 0;
    for (int y = (height / 8) - 1; y >= 0; y--) {
        for (int x = 0; x < width / 8; x++) {
            if (containsBlock(x, y)) {
                if (emptyLines > 0) {
                    moveRowDown(y, emptyLines);
                }
                break;
            } else if (x == width / 8 - 1) {
                emptyLines++;
            }
        }
    }
}

void Board::moveRowDown(int row, int numLines) {
    for (Tetromino *piece : tetrominos) {
        if (piece != activeTetromino && piece != nextTetromino) {
            for (auto [block, blockPos] : piece->textures) {
                if (piece->position.y / 8 + blockPos.y / 8 == row) {
                    for(auto const& [collider, colPos] : piece->colliders) {
                        collider->box->y += 8 * numLines;
                    }
                    piece->colliderPosition.y += 8 * numLines;
                    piece->position.y += 8 * numLines;
                }
            }
        }
    }
}

void Board::removeClearedPieces() {
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

void Board::prepAnimations() {
    for (int i = 0; i < height / 8; i++) {
        animations.push_back(new Animation(renderer, "resources/textures/anim_clear_h.png", 7, {0, i}));
    }
}

bool Board::animationsPlaying() {
    for (Animation* animation : animations) {
        if (animation->playing) {
            return true;
        }
    }
    return false;
}

