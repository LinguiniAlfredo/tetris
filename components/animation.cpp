#include "animation.h"

Animation::Animation(SDL_Renderer *renderer, string filepath) {
    this->renderer = renderer;
    this->texture = new Texture(renderer, filepath, 0, 0);
    this->stencil = new SDL_Rect {
        0, 0, 
        this->texture->width / numFrames, this->texture->height 
    };
}

Animation::~Animation() {
    if (texture != nullptr) {
        delete texture;
    }
    if (stencil != nullptr) {
        delete stencil;
    }
}

void Animation::update() {
    stencil->x += texture->width / numFrames;
    if (stencil->x >= texture->width) {
        stencil->x = 0;
    }
}

void Animation::draw() {
    texture->renderClipped(0, 0, stencil);
}


void Animation::start() {

}

void Animation::stop() {
}

