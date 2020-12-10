#pragma once

#include <functional>
#include "component.h"
#include "components/emitter.h"

/**
 * Destroys the actor of a set duration
 */
class Particle : public Component {
	public:
		const std::function<bool(Actor&, float, float)> decay;
		const float birthtime;
		Particle(const Emitter &emitter);
};