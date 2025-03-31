#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../components/texture.h"
#include "../utils/vec2.h"


class HUD
{
public:
    HUD();
	HUD(SDL_Renderer *renderer);
	~HUD();

	TTF_Font *font = nullptr;
	Texture *fps_texture = nullptr;
	SDL_Color color = { 0, 0, 0, 255 };
    
    // TODO - make window size globally included
    Vec2 fpsPosition = { 11, 19 };

	void update(float fps);
	void draw() const;

private:
	void updateFPS(float value);
    void updatePoints(float value);
};
