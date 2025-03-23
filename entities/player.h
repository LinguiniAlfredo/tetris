#pragma once
#include "SDL2/SDL.h"
#include "../components/texture.h"

class Player 
{
public:
    Player(SDL_Renderer *renderer);
    ~Player();

    Texture *texture;

    void update();
    void draw() const;

};