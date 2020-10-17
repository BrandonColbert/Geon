#include "systems/motion_system.h"

#include <algorithm>
#include "components/motion.h"
#include "components/rect.h"
#include "engine.h"
#include "math/vector2.h"

using namespace std;
using Actors = Engine::Actors;
using Time = Engine::Time;

void MotionSystem::update() {
	Actors::forEach<Motion, Rect>([](Actor &actor) {
		auto &motion = actor.get<Motion>();
		auto &rect = actor.get<Rect>();

		motion.velocity += motion.acceleration;
		motion.acceleration = Vector2::zero;

		if(motion.terminalVelocity.sqr_magnitude() > 0)
			motion.velocity = Vector2(
				clamp(motion.velocity.x, -motion.terminalVelocity.x, motion.terminalVelocity.x),
				clamp(motion.velocity.y, -motion.terminalVelocity.y, motion.terminalVelocity.y)
			);

		rect.position += motion.velocity * Time::deltaTime;

		motion.velocity *= motion.friction;
	});
}