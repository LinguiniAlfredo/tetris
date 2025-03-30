#include "tetromino.h"
#include "../board.h"

using namespace std;

Tetromino::Tetromino(SDL_Renderer *renderer, TetrominoType type, Vec2 position) {
    this->type = type;
    this->texture = getTetrominoTexture(type, renderer);
    this->position = position;
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
                break;
            case SDLK_d:
                position.x += 8;
                if (!inBounds()) {
                    position.x -= 8;
                }
                break;
            default:
                break;
        }
    }
}

void Tetromino::update() {
    position.y += 1;
    if (!inBounds()) {
        position.y -= 1;
    }
}

void Tetromino::draw() const {
    if (texture != nullptr) {
        texture->render(position.x, position.y);
    }
}

void Tetromino::rotate() {

}

void Tetromino::drop() {

}

bool Tetromino::inBounds() {
    return position.x >= 0 && 
           position.y >= 0 && 
           position.x <= board->width - texture->width && 
           position.y <= board->height - texture->height;
}

Texture* Tetromino::getTetrominoTexture(TetrominoType type, SDL_Renderer *renderer) {
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
