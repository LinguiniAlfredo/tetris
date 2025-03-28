#pragma once
#include "../components/texture.h"

using namespace std;

typedef enum {
    I,
    O,
    T,
    S,
    Z,
    J,
    L
} TetrominoType;

class Tetromino 
{
public:
    Tetromino(SDL_Renderer *renderer, TetrominoType type, int x, int y);
    ~Tetromino();

    TetrominoType type;
    Texture *texture = nullptr;
    int x = 0;
    int y = 0;
    // Vec2 position;
    // Vec2 center;

    void update();
    void draw() const;

    void rotate();
    void drop();

private:
    Texture* getTetrominoTexture(TetrominoType type, SDL_Renderer *renderer);
};