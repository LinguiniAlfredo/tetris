#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class Texture;

class HUD
{
public:
	HUD(SDL_Renderer* renderer);
	~HUD();

	SDL_Renderer* renderer;
	TTF_Font* font;
	Texture* fps_texture;
	SDL_Color color = { 255,255,255,255 };

	void updateAndDraw(float fps);

private:
	void updateFPS(float value);
};