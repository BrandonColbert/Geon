#pragma once

#include "component.h"
#include "structures/vector2.h"

/**
 * Makes an object's movement controllable
 */
class Controller : public Component {
	public:
		/**
		 * Movement speed
		 */
		float speed;
		Vector2 move, lastMove;

		/**
		 * @param speed Movement speed
		 */
		Controller(float speed);
};