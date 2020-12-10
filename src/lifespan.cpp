#include "components/lifespan.h"

#include "engine.h"

using Time = Engine::Time;

Lifespan::Lifespan(float duration) {
	epoch = Time::time + duration;
}

float Lifespan::getEpoch() {
	return epoch;
}

bool Lifespan::alive() {
	return Time::time < epoch;
}