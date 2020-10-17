#pragma once

#include <functional>
#include "actor.h"
#include "component.h"

class Spawner : public Component {
	public:
		/** Spawns a single actor */
		std::function<Actor&()> spawn;

		/** Whether to spawn this frame */
		std::function<bool()> condition;

		Spawner();
};