#include "systems/particle_system.h"

#include <limits>
#include "components/emitter.h"
#include "components/lifespan.h"
#include "components/particle.h"
#include "engine.h"

using namespace std;
using Actors = Engine::Actors;
using Time = Engine::Time;

void ParticleSystem::update() {
	Actors::forEach<Emitter>([](Actor &actor) {
		auto &emitter = actor.get<Emitter>();
		emitter.emit();
	});

	Actors::forEach<Particle>([](Actor &actor) {
		auto &particle = actor.get<Particle>();

		if(!particle.decay)
			return;

		auto shouldDestroy = particle.decay(
			actor,
			actor.has<Lifespan>() ? actor.get<Lifespan>().getEpoch() - particle.birthtime : numeric_limits<float>::infinity(),
			Time::time - particle.birthtime
		);

		if(shouldDestroy)
			actor.destroy();
	});
}