#pragma once

#include "component.h"

class Lifespan : public Component {
	public:
		/**
		 * @param duration Duration in seconds before destruction
		 */
		Lifespan(float duration);

		/**
		 * @return Whether the lifespan has ended
		 */
		bool alive();
	private:
		float epoch;
};