#pragma once
#include <SDL2/SDL.h>

class Collision {
public:
    Collision(SDL_Renderer *renderer, int x, int y, int w, int h);
    ~Collision();

    SDL_Renderer *renderer = nullptr;
    SDL_Rect *box = nullptr;
};
