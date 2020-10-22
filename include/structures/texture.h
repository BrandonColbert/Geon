#pragma once

#include <SDL2/SDL_image.h>
#include <string>
#include "structures/color.h"

/**
 * Holds an image texture
 */
class Texture {
	public:
		/** Path to the image */
		const std::string path;

		/**
		 * @param path Path to source image
		 */
		Texture(std::string path);

		/**
		 * @param width Texture width
		 * @param height Texture height
		 * @param color Fill color
		 */
		Texture(int width, int height, Color color);

		Texture(const Texture&);
		Texture(Texture&&);

		Texture();
		~Texture();

		int getWidth();
		int getHeight();

		operator SDL_Surface*() const;
		operator SDL_Texture*() const;
		explicit operator bool() const;
	private:
		SDL_Surface *surface;
		SDL_Texture *texture;
		int width, height;
};