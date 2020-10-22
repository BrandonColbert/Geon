#pragma once

#include "structures/vector2.h"

/**
 * Axis-Aligned Bounding Box
 */
class BoundingBox {
	public:
		Vector2 position, size;

		BoundingBox(Vector2 position, Vector2 size);

		/**
		 * @param other BoundingBox to check overlap with
		 * @returns Whether this bounding box is overlapping with the specified bounding box
		 */
		bool overlaps(const BoundingBox &other) const;

		/**
		 * @param other Intruding box
		 * @return Velocity for the specified box to move out of this box
		 */
		Vector2 expulsion(const BoundingBox &other) const;
};