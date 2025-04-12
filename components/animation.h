#pragma once
#include "SDL2/SDL.h"
#include "texture.h"
#include <string>
#include "../utils/vec2.h"

using namespace std;

class Animation {

public:
    Animation(SDL_Renderer *renderer, string filepath);
    Animation(SDL_Renderer *renderer, string filepath, int numFrames, Vec2 position);
    ~Animation();

    SDL_Renderer *renderer = nullptr;
    Texture *texture = nullptr;
    SDL_Rect *stencil = nullptr;

    Vec2 position = { 0, 0 };

    int numFrames = 10;
    int duration = 0;

    bool playing = false;

    void update(int currentFrame);
    void draw();

    void play();

    void pause();
    void stop();


};
