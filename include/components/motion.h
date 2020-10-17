#pragma once

#include "component.h"
#include "math/vector2.h"

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

		/** How quickly the object slows down */
		float friction;
};