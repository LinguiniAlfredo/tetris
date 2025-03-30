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
    Tetromino(SDL_Renderer *renderer, TetrominoType type, Vec2 position);
    ~Tetromino();

    TetrominoType type;
    Texture *texture = nullptr;
    Board *board = nullptr;
    Vec2 position;
    Vec2 velocity;
    // Vec2 center;

    void handleEvent(const SDL_Event& e);
    void update();
    void draw() const;
    void rotate();
    void drop();
    bool inBounds();

private:
    Texture* getTetrominoTexture(TetrominoType type, SDL_Renderer *renderer);

};
