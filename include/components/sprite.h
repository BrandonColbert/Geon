#pragma once

#include "component.h"
#include "graphics/texture.h"
#include "math/vector2.h"

/**
 * Allows an object to be rendered
 */
class Sprite : public Component {
	public:
		/** Texture to use for rendering */
		Texture texture;

		/** Center point relative to texture */
		Vector2 pivot;

		/** Texture starting point */
		Vector2 uv;

		/** Texture width and height */
		Vector2 size;

		/** Whether to flip horizontally */
		bool flipX = false;

		/** Whether to flip vertically */
		bool flipY = false;

		/** Depth relative to other rendered objects */
		float depth = 0;

		/**
		 * @param texture Texture to use
		 */
		Sprite(Texture texture, float depth = 0);
		Sprite();
};