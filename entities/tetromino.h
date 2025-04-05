#pragma once
#include "../utils/vec2.h"
#include "../components/texture.h"
#include <map>
#include <vector>
#include <string>

class Board;
class Collision;

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
 
    TetrominoType type;
    Board *board = nullptr;
    map<Texture*, Vec2> textures;
    vector<Collision*> colliders;

    Vec2 position;
    Vec2 velocity = {0, 0};
    double trueYPos;

    bool checkCollisions();
    void handleEvent(const SDL_Event& e);
    void update();
    void draw() const;
    void rotate();
    void drop();
    void moveX();
    void checkLock(Vec2 initialPosition);

    bool inBounds();
    bool colliding = false;

private:
    void acquireTetrominoTextures(TetrominoType type, SDL_Renderer *renderer);
    void constructLTexture(SDL_Renderer *renderer, string filepath);
    void generateColliders(SDL_Renderer *renderer);
};
