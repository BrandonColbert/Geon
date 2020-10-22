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
	vector<Actor*> actors;
	vector<BoundingBox> bbs;

	Actors::forEach<Rect, Collider>([&](Actor &actor) {
		actors.push_back(&actor);
	});

	bbs.reserve(actors.size());
	for(auto actor : actors)
		bbs.push_back(BoundingBox(
			actor->get<Rect>().position,
			actor->get<Collider>().size
		));

	for(auto i = (int)actors.size() - 1; i > 0; i--) {
		auto &actor = *actors[i];
		auto &bb = bbs[i];

		for(auto j = i - 1; j >= 0; j--) {
			auto &otherActor = *actors[j];
			auto &otherBB = bbs[j];

			if(!bb.overlaps(otherBB))
				continue;

			if(actor.has<Motion>())
				actor.get<Motion>().velocity += otherBB.expulsion(bb);

			if(otherActor.has<Motion>())
				otherActor.get<Motion>().velocity += bb.expulsion(otherBB);
		}
	}
}