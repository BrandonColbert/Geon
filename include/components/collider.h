#pragma once

#include "component.h"
#include "structures/vector2.h"

/**
 * Allows an object to be collided with
 */
class Collider : public Component {
	public:
		/** Collision region size */
		Vector2 size;

		/** Coefficient of restitution */
		float restitution = 1;

		/**
		 * @param size Collision region size
		 */
		Collider(Vector2 size);
};