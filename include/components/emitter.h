#pragma once

#include <functional>
#include "actor.h"
#include "component.h"
#include "structures/texture.h"
#include "structures/vector2.h"

/**
 * Particle emitter
 */
class Emitter : public Component {
	public:
		/**
		 * Maximum amount of particles that may exist at once for this emitter
		 * 
		 * If less than 0, no limit is applied
		 */
		int maxParticles = -1;

		/** Render depth of emitted particles */
		float depth = 0;

		/** How frequently particles are emitted every second */
		float frequency = 1;

		/** Returns how long a particle will exist */
		std::function<float()> particleLifespan;

		/** Returns where an emitted particle spawns */
		std::function<Vector2()> emitPosition;

		/** Returns the size of a particle when spawned */
		std::function<Vector2()> emitSize;

		/** Return the initial velocity of a particle */
		std::function<Vector2()> emitVelocity;

		/** Return the texture of a particle */
		std::function<Texture()> emitTexture;

		/**
		 * Changes a particle over its lifetime
		 * 
		 * Arguments are the particle's actor, lifespan (expected duration), and lifetime (time spent alive)
		 * Return true to immediately destroy the particle
		 */
		std::function<bool(Actor&, float, float)> decay;

		/**
		 * Emits a particle if possible
		 */
		void emit();
	private:
		float nextEmitTime = 0;
};