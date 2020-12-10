#include "systems/mob_system.h"

#include <cmath>
#include "console.h"
#include "engine.h"
#include "components/mob.h"

using namespace std;
using Actors = Engine::Actors;
using Time = Engine::Time;

int MobSystem::tickRate = 20;
unordered_map<std::string, MobSystem::Behavior> MobSystem::behaviors;

float delta = 0;
int tick = 0;

#include "console.h"

void MobSystem::update() {
	delta += Time::deltaTime;
	auto alpha = 1.0f / tickRate;

	if(delta >= alpha) {
		// auto promisedTicks = (int)floor(delta / alpha);

		// if(promisedTicks > 1)
		// 	Console::log("Skipped % behavior update", promisedTicks - 1);

		delta = fmod(delta, alpha);
		tick++;

		Actors::forEach<Mob>([](Actor &actor) {
			auto type = actor.get<Mob>().getType();
			auto behavior = behaviors[type];
			behavior(actor, tick);
		});
	}
}

void MobSystem::setBehavior(string type, Behavior behavior) {
	auto it = behaviors.find(type);

	if(it != behaviors.end()) {
		Console::error("Behavior already exists for mob type '%'", type);
		return;
	}

	behaviors[type] = behavior;
}

void MobSystem::deleteBehavior(string type) {
	auto it = behaviors.find(type);

	if(it == behaviors.end())
		return;

	behaviors.erase(it);
}

bool MobSystem::typeExists(string type) {
	return behaviors.find(type) != behaviors.end();
}