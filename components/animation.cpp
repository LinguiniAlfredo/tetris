#include "animation.h"

Animation::Animation(SDL_Renderer *renderer, string filepath) {
    this->renderer = renderer;
    this->texture = new Texture(renderer, filepath, 0, 0);
    this->stencil = new SDL_Rect {
        0, 0, 
        this->texture->width / numFrames, this->texture->height 
    };
}

Animation::Animation(SDL_Renderer *renderer, string filepath, int numFrames, Vec2 position) {
    this->renderer = renderer;
    this->texture = new Texture(renderer, filepath, 0, 0);
    this->stencil = new SDL_Rect {
        0, 0, 
        this->texture->width / numFrames, this->texture->height 
    };
    this->numFrames = numFrames;
    this->position = position;
}

Animation::~Animation() {
    if (texture != nullptr) {
        delete texture;
    }
    if (stencil != nullptr) {
        delete stencil;
    }
}

void Animation::update(int currentFrame) {
    if (currentFrame % 2 == 0) {
        stencil->x += texture->width / numFrames;
        if (stencil->x >= texture->width) {
            stencil->x = 0;
            stop();
        }
    }
}

void Animation::draw() {
    texture->renderClipped(position.x, position.y, stencil);
}


void Animation::play() {
    this->playing = true;
}

void Animation::pause() {
}

void Animation::stop() {
    this->playing = false;
}

