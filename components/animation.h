#pragma once
#include "SDL2/SDL.h"
#include "texture.h"
#include <string>

using namespace std;

class Animation {

public:
    Animation(SDL_Renderer *renderer, string filepath);
    ~Animation();

    SDL_Renderer *renderer = nullptr;
    Texture *texture = nullptr;
    SDL_Rect *stencil = nullptr;

    int numFrames = 4;

    void update();
    void draw();

    void start();
    void stop();

};
