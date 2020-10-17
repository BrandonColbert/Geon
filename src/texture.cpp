#include "graphics/texture.h"

#include <cstdint>
#include <utility>
#include "engine.h"
#include "console.h"

using namespace std;

Texture::Texture(string path) : path(path) {
	surface = IMG_Load(path.c_str());

	if(surface == NULL) {
		Console::error("Texture: %", IMG_GetError());
		return;
	}

	texture = SDL_CreateTextureFromSurface(Engine::getInstance(), surface);
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);
}

#include <iostream>

Texture::Texture(int width, int height, Color color) : path(""), width(width), height(height) {
	surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	if(surface == NULL) {
		Console::error("Texture: %", SDL_GetError());
		return;
	}

	auto argb = color.getARGB();
	auto a = (argb >> 24) & 0xff;
	auto r = (argb >> 16) & 0xff;
	auto g = (argb >> 8) & 0xff;
	auto b = argb & 0xff;
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, a));

	texture = SDL_CreateTextureFromSurface(Engine::getInstance(), surface);
}

Texture::Texture() : path("") {
	width = height = 0;
	surface = nullptr;
	texture = nullptr;
}

Texture::Texture(const Texture &other) : path(other.path), width(other.width), height(other.height) {
	surface = SDL_ConvertSurface(other.surface, other.surface->format, 0);
	SDL_BlitSurface(other.surface, NULL, surface, NULL);

	texture = SDL_CreateTextureFromSurface(Engine::getInstance(), surface);
}

Texture::Texture(Texture &&other) : path(move(other.path)) {
	width = exchange(other.width, width);
	height = exchange(other.height, height);
	surface = exchange(other.surface, nullptr);
	texture = exchange(other.texture, nullptr);
}

Texture::~Texture() {
	if(surface != nullptr)
		SDL_FreeSurface(surface);
	
	if(texture != nullptr)
		SDL_DestroyTexture(texture);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

Texture::operator SDL_Surface*() const {
	return surface;
}

Texture::operator SDL_Texture*() const {
	return texture;
}

Texture::operator bool() const {
	return surface != nullptr && texture != nullptr;
}