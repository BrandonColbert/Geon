#pragma once

#include "component.h"
#include "structures/vector2.h"

/**
 * Enables an object to have physically based movement
 */
class Motion : public Component {
	public:
		/** Current velocity of the object */
		Vector2 velocity;

		/** Current acceleration acting on the object */
		Vector2 acceleration;

		/**
		 * Maximum velocity the object can reach
		 * 
		 * If magnitude is 0, no terminal velocity applied
		 */
		Vector2 terminalVelocity;

		/**
		 * How quickly the object slows down
		 * 
		 * On a scale from 0 to 1
		 */
		float friction;
};