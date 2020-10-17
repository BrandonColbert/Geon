#include "systems/lifespan_system.h"

#include "components/lifespan.h"
#include "engine.h"

using Actors = Engine::Actors;

void LifespanSystem::update() {
	Actors::forEach<Lifespan>([](Actor &actor) {
		auto &lifespan = actor.get<Lifespan>();

		if(!lifespan.alive())
			actor.destroy();
	});
}