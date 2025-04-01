#include "collision.h"

Collision::Collision(SDL_Renderer *renderer, int x, int y, int width, int height) {
    this->renderer;
    this->box = new SDL_Rect{x, y, width, height};
}

Collision::~Collision() {
    delete box;
}
