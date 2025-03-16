#pragma once
#include "texture.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Texture::Texture(SDL_Renderer* r, string path, int w, int h) {
	renderer = r;
	width = w;
	height = h;
	texture = loadFromFile(path);
    name = path;
}

Texture::Texture(SDL_Renderer* r, string text, TTF_Font* font, SDL_Color color, int w, int h) {
	renderer = r;
	width = w;
	height = h;
	texture = loadFromRenderedText(text, font, color);
}

Texture::~Texture() {
	free();
}

void Texture::render(int x, int y) {
	SDL_Rect renderQuad = { x, y, width, height };
	SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
}

SDL_Texture* Texture::loadFromFile(string path) {
	free();

	SDL_Texture* newTexture = nullptr;

	SDL_Surface* surface = IMG_Load(path.c_str());	

	if (surface == nullptr) {
		printf("unable to load image %s : %s\n", path.c_str(), IMG_GetError());
	}
	else {
		// Uncomment for transparency keyframing
		//SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(renderer, surface);
		if (newTexture == nullptr) {
			printf("unable to create texture %s : %s\n", path.c_str(), SDL_GetError());
		}
		else {
			if (width == 0 && height == 0) {
				width = surface->w;
				height = surface->h;
			}
		}
		SDL_FreeSurface(surface);
	}
	return newTexture;
}

SDL_Texture* Texture::loadFromRenderedText(string text, TTF_Font* font, SDL_Color color) {
	free();

	SDL_Texture* newTexture = nullptr;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (textSurface == nullptr) {
		printf("unable to render text %s\n", TTF_GetError());
	}
	else {
		newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (newTexture == nullptr) {
			printf("unable to create text texture\n");
		}
		else {
			if (width == 0 && height == 0) {
				width = textSurface->w;
				height = textSurface->h;
			}
		}

		SDL_FreeSurface(textSurface);
	}

	return newTexture;
}

void Texture::free() {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = 0;
		height = 0;
	}
}