#pragma once
#include "../utils/vec2.h"
#include "../components/texture.h"

class Board;

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
    Tetromino(SDL_Renderer *renderer, Board *board, TetrominoType type, Vec2 position);
    ~Tetromino();

    Board *board = nullptr;
    Texture *texture = nullptr;
    TetrominoType type;

    Vec2 position;
    double trueYPos;

    void handleEvent(const SDL_Event& e);
    void update();
    void draw() const;
    void rotate();
    void drop();
    bool inBounds();

private:
    Texture* acquireTetrominoTexture(TetrominoType type, SDL_Renderer *renderer);
};
