#pragma once

#include "component.h"

/**
 * Destroys the actor of a set duration
 */
class Lifespan : public Component {
	public:
		/**
		 * @param duration Duration in seconds before destruction
		 */
		Lifespan(float duration);

		/**
		 * @return End of the lifetime
		 */
		float getEpoch();

		/**
		 * @return Whether the lifespan has ended
		 */
		bool alive();
	private:
		float epoch;
};