#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../components/texture.h"


class HUD
{
public:
    HUD();
	HUD(SDL_Renderer *renderer);
	~HUD();

	TTF_Font *font = nullptr;
	Texture *fps_texture = nullptr;
	SDL_Color color = { 0,0,0,255 };

	void update(float fps);
	void draw() const;

private:
	void updateFPS(float value);
};
