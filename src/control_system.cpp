#include "systems/control_system.h"

#include "components/controller.h"
#include "components/motion.h"
#include "components/rect.h"
#include "engine.h"

using namespace std;
using Actors = Engine::Actors;
using Time = Engine::Time;

void ControlSystem::update() {
	Actors::forEach<Controller>([](Actor &actor) {
		auto &controller = actor.get<Controller>();

		controller.lastMove = controller.move;

		if(actor.has<Motion>()) {
			auto &motion = actor.get<Motion>();
			motion.acceleration = controller.move * controller.speed;
		} else if(actor.has<Rect>()) {
			auto &rect = actor.get<Rect>();
			rect.position += controller.move * controller.speed * Time::deltaTime;
		}
	});
}