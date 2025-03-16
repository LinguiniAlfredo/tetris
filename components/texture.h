#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

class Texture
{
public:
	Texture(SDL_Renderer* renderer, string path, int width, int height);
	Texture(SDL_Renderer* r, string text, TTF_Font* font, SDL_Color color, int w, int h);
	~Texture();

	int width;
	int height;
    string name = "";
	SDL_Texture* texture;
	SDL_Renderer* renderer;

	void render(int x, int y);

private:
	SDL_Texture* loadFromFile(string path);
	SDL_Texture*  loadFromRenderedText(string text, TTF_Font* font, SDL_Color color);
	void free();
};