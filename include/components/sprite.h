#pragma once

#include "component.h"
#include "structures/texture.h"
#include "structures/vector2.h"

/**
 * Allows an object to be rendered
 */
class Sprite : public Component {
	public:
		/** Color to tint the texture when rendering */
		Color tint = Color::white;

		/** Texture to use for rendering */
		Texture texture;

		/** Texture starting point */
		Vector2 uv;

		/** Texture width and height */
		Vector2 size;

		/** Center point relative to texture */
		Vector2 pivot;

		/** Whether to flip horizontally */
		bool flipX = false;

		/** Whether to flip vertically */
		bool flipY = false;

		/** Render offset */
		Vector2 offset;

		/** Depth relative to other rendered objects */
		float depth;

		/**
		 * @param texture Texture to use
		 * @param depth Visual depth
		 */
		Sprite(Texture texture, float depth = 0);

		/**
		 * @param depth Visual depth
		 */
		Sprite(float depth);

		Sprite();
};