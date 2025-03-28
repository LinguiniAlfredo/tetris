#include "tetromino.h"

Tetromino::Tetromino(SDL_Renderer *renderer, TetrominoType type, int x, int y) {
    this->type = type;
    this->texture = getTetrominoTexture(type, renderer);
    this->x = x;
    this->y = y;
}

Tetromino::~Tetromino() {
    if (texture != nullptr) {
        delete texture;
        texture = nullptr;
    }
}

void Tetromino::update() {

}

void Tetromino::draw() const {
    if (texture != nullptr) {
        texture->render(x, y);
    }
}

void Tetromino::rotate() {

}

void Tetromino::drop() {

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