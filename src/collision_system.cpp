#include "systems/collision_system.h"

#include <vector>
#include "components/collider.h"
#include "components/motion.h"
#include "components/rect.h"
#include "engine.h"
#include "structures/bounding_box.h"
#include "structures/vector2.h"

using namespace std;
using Actors = Engine::Actors;

void CollisionSystem::update() {
	vector<Actor*> mobile, immobile;

	//Acquire collidable actors, reset collision state, and update bounds
	Actors::forEach<Rect, Collider>([&](Actor &actor) {
		auto &collider = actor.get<Collider>();
		collider.isCollided = false;
		collider.recalculateBounds();

		(actor.has<Motion>() ? mobile : immobile).push_back(&actor);
	});

	//Check for collisions between mobile objects
	for(auto i = (int)mobile.size() - 1; i > 0; i--) {
		auto &actor = *mobile[i];
		auto &collider = actor.get<Collider>();

		for(auto j = i - 1; j >= 0; j--) {
			auto &other = *mobile[j];
			auto &otherCollider = other.get<Collider>();

			//Check overlap
			if(!collider.bounds.overlaps(otherCollider.bounds))
				continue;

			//Respond
			collider.respond(actor, other);
			otherCollider.respond(other, actor);
		}
	}

	//Check for collisions between mobile and immobile objects
	for(auto mobileActor : mobile) {
		auto &mobileCollider = mobileActor->get<Collider>();

		for(auto immobileActor : immobile) {
			auto &immobileCollider = immobileActor->get<Collider>();

			//Check overlap
			if(!mobileCollider.bounds.overlaps(immobileCollider.bounds))
				continue;

			//Respond
			mobileCollider.respond(*mobileActor, *immobileActor);
		}
	}
}