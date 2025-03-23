#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../components/texture.h"


class HUD
{
public:
	HUD(SDL_Renderer *renderer);
	~HUD();

	TTF_Font *font;
	Texture *fps_texture;
	SDL_Color color = { 255,255,255,255 };

	void update(float fps);
	void draw() const;

private:
	void updateFPS(float value);
};