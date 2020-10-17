#pragma once

#include "component.h"
#include "math/vector2.h"
#include "utils/property.h"

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

		/**
		 * @param other Rect to check overlap with
		 * @returns Whether this rect is overlapping with the given rect
		 */
		bool isOverlapping(const Rect &other) const;
};