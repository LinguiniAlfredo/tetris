#include "player.h"

Player::Player(SDL_Renderer *renderer) {
    this->texture = new Texture(renderer, "resources/textures/block.png", 0, 0);
}

Player::~Player() {
    if (texture != nullptr) {
        delete texture;
    }
}

void Player::update() {

}

void Player::draw() const {
    this->texture->render(10, 10);
}