#include "components/particle.h"

#include "engine.h"

using Time = Engine::Time;

Particle::Particle(const Emitter &emitter) : decay(emitter.decay), birthtime(Time::time) {}