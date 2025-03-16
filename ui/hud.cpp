#include "hud.h"
#include "../components/texture.h"

HUD::HUD(SDL_Renderer* renderer) {
	this->renderer = renderer;
	this->font = TTF_OpenFont("resources/fonts/nintendo-nes-font.ttf", 28);
	if (font == NULL) {
		printf("error loading font: %s \n", TTF_GetError());
		font = nullptr;
	}
	this->fps_texture = new Texture(renderer, "0 FPS", font, color, 15,5);
}

HUD::~HUD() {
	delete fps_texture;
	fps_texture = nullptr;

	TTF_CloseFont(font);
	font = nullptr;
}

void HUD::updateAndDraw(float fps) {
	updateFPS(fps);
	//updateCoins()
	//updateLife()
	this->fps_texture->render(1, 1);
	//this->coins->render
	//this->life->render
}

void HUD::updateFPS(float value) {
	if (fps_texture != nullptr) {
		delete fps_texture;
	}
	fps_texture = new Texture(renderer, std::to_string((int)round(value)) + " FPS", font, color, 15, 5);
}