#include "components/spawner.h"

#include "actor.h"

Spawner::Spawner() {
	spawn = []() -> Actor& {
		return Actor::spawn();
	};

	condition = []() -> bool {
		return false;
	};
}