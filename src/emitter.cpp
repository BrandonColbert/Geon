#include "components/emitter.h"

#include "components/lifespan.h"
#include "components/motion.h"
#include "components/particle.h"
#include "components/rect.h"
#include "components/sprite.h"
#include "engine.h"

using namespace std;
using Time = Engine::Time;

void Emitter::emit() {
	// if(maxParticles >= 0 && (int)birthtimes.size() >= maxParticles)
	// 	return;

	if(Time::time < nextEmitTime)
		return;

	auto &particle = Actor::spawn("Particle");
	particle.add(new Particle(*this));
	particle.add(new Rect(Vector2::zero, Vector2::one));
	particle.add(new Motion());

	if(particleLifespan)
		particle.add(new Lifespan(particleLifespan()));

	if(emitPosition)
		particle.get<Rect>().position = emitPosition();

	if(emitSize)
		particle.get<Rect>().size = emitSize();

	if(emitVelocity)
		particle.get<Motion>().velocity = emitVelocity();

	if(emitTexture)
		particle.add(new Sprite(emitTexture(), depth));

	nextEmitTime = Time::time + 1.0f / frequency;
}