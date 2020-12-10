#include "scenes/dungeon/health.h"

#include <algorithm>
#include "engine.h"

using namespace std;
using Time = Engine::Time;

void defaultDeath(Actor &actor) {
	actor.destroy();
}

Health::Health(float value) : onDeath(defaultDeath), value(value), maxValue(value) {}

float Health::current() {
	return value;
}

float Health::max() {
	return maxValue;
}

void Health::damage(float amount) {
	if(value == 0 || Time::time <= (lastHitTime + invincibilityDuration))
		return;

	lastHitTime = Time::time;

	auto lastValue = value;
	value = std::max(0.0f, value - amount);

	if(onChange)
		onChange(getActor(), lastValue, value);

	if(value == 0 && getActor().exists() && onDeath)
		onDeath(getActor());
}

void Health::heal(float amount) {
	auto lastValue = value;
	value = min(maxValue, value + amount);

	if(onChange)
		onChange(getActor(), lastValue, value);
}

void Health::kill() {
	if(value == 0)
		return;

	damage(value);
}