#include "systems/spawner_system.h"

#include "components/spawner.h"
#include "engine.h"

using Actors = Engine::Actors;

void SpawnerSystem::update() {
	Actors::forEach<Spawner>([](Actor &actor) {
		auto &spawner = actor.get<Spawner>();

		if(spawner.condition())
			spawner.spawn();
	});
}