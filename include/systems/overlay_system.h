#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "structures/color.h"
#include "structures/vector2.h"
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
		 */
		static void text(std::string text, Vector2 position, Color color = Color::white);
	private:
		static std::vector<Overlay*> overlays;
};