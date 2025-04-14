#pragma once
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../components/texture.h"
#include "../utils/vec2.h"

#include "../gamestate.h"
extern GameState gameState;

class HUD
{
public:
    HUD();
	HUD(SDL_Renderer *renderer);
	~HUD();

	TTF_Font *font = nullptr;
	Texture *score_texture = nullptr;
	Texture *fps_texture = nullptr;
	SDL_Color color = { 0, 0, 0, 255 };
    
    // TODO - make window size globally included
    Vec2 fpsPosition = { 11, 19 };
	Vec2 scorePosition = { 11, 17 };

	void update(double score, float fps);
	void draw() const;

private:
    void updateScore(double score);
	void updateFPS(float value);
};

