#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "structures/color.h"
#include "structures/vector2.h"
#include "structures/texture.h"
#include "system.h"

/**
 * Basic overlay element
 */
class Overlay {
	public:
		virtual ~Overlay();
		virtual void render() = 0;
};

/**
 * Enables temporary, non-interactable UI elements
 */
class OverlaySystem : public System {
	public:
		OverlaySystem();
		~OverlaySystem();
		void render() override;

		/**
		 * Displays text
		 * @param text Text to display
		 * @param position Position to display text at
		 * @param color Color of the text
		 * @param scale Scale of the font
		 */
		static void text(std::string text, Vector2 position, Color color = Color::white, float scale = 1);

		/**
		 * Displays a colored box
		 * @param position Box center
		 * @param size Box size
		 * @param color Color of the box
		 */
		static void box(Vector2 position, Vector2 size, Color color);

		/**
		 * Displays a texture
		 * @param texture Texture
		 * @param position Center of the texture
		 * @param scale Texture scale
		 */
		static void texture(Texture texture, Vector2 position, Vector2 scale = Vector2::one);
	private:
		static std::vector<Overlay*> overlays;
};