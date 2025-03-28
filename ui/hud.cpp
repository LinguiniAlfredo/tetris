#include "hud.h"

HUD::HUD(SDL_Renderer *renderer) {
	this->font = TTF_OpenFont("resources/fonts/nes.ttf", 28);
	this->fps_texture = new Texture(renderer, "0 FPS", font, color, 15,5);
}

HUD::~HUD() {
	if (fps_texture != nullptr) {
		delete fps_texture;
		fps_texture = nullptr;
	}

	if (font != nullptr) {
		TTF_CloseFont(font);
		font = nullptr;
	}
}

void HUD::update(float fps) {
	updateFPS(fps);
	//updateCoins()
	//updateLife()
}

void HUD::draw() const {
	if (fps_texture != nullptr) {
		fps_texture->render(1,1);
	}
	//coins_texture->render();
	//life_texture->render();
}

void HUD::updateFPS(float value) {
	SDL_Renderer *tmp;
	if (fps_texture != nullptr) {
		tmp = fps_texture->renderer;
		delete fps_texture;
	}
	fps_texture = new Texture(tmp, std::to_string((int)round(value)) + " FPS", font, color, 15, 5);
}