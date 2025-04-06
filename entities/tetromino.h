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
    map<Texture*, Vec2> ghostTextures;
    map<Collision*, Vec2> colliders;
    vector<Texture*> textureTrash;
    vector<Collision*> colliderTrash;

    Vec2 position;
    Vec2 ghostPosition;
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
    bool softDropStart = false;
    bool instaLock = false;

private:
    void constructTetromino(TetrominoType type, SDL_Renderer *renderer);
    void generateColliders(SDL_Renderer *renderer);
    void findGhostPosition();
    void hardDrop();
};
