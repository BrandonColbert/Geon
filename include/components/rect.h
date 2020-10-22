#pragma once

#include "component.h"
#include "structures/vector2.h"

/**
 * Defines the position, size, and depth of an object
 */
class Rect : public Component {
	public:
		/** Position of the object */
		Vector2 position;

		/** Size of the object */
		Vector2 size;

		/** Angle of rotation in degrees */
		float rotation = 0;

		Rect(Vector2 position, Vector2 size);
};