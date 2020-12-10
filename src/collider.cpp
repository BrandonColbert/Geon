#include "components/collider.h"

#include <algorithm>
#include <cmath>
#include "components/rect.h"

using namespace std;

void clip(Actor &actor, Actor &hit) {
	//TODO: Fix no-clipping when centers are exactly aligned on an axis

	auto &collider = actor.get<Collider>();
	auto &motion = actor.get<Motion>();
	auto &rect = actor.get<Rect>();

	auto bounds = collider.bounds;
	collider.isCollided = true;

	auto hitBounds = hit.get<Collider>().bounds;

	//Find expel values
	auto expelTop = max(0.0f, ((bounds.position.y + bounds.size.y / 2) - (hitBounds.position.y - hitBounds.size.y / 2)) * (bounds.position.y < hitBounds.position.y ? 1 : -1));
	auto expelBot = max(0.0f, ((hitBounds.position.y + hitBounds.size.y / 2) - (bounds.position.y - bounds.size.y / 2)) * (bounds.position.y > hitBounds.position.y ? 1 : -1));
	auto expelLeft = max(0.0f, ((hitBounds.position.x + hitBounds.size.x / 2) - (bounds.position.x - bounds.size.x / 2)) * (bounds.position.x > hitBounds.position.x ? 1 : -1));
	auto expelRight = max(0.0f, ((bounds.position.x + bounds.size.x / 2) - (hitBounds.position.x - hitBounds.size.x / 2)) * (bounds.position.x < hitBounds.position.x ? 1 : -1));

	//Determine x and/or y collision
	auto expelX = expelLeft + expelRight;
	auto expelY = expelTop + expelBot;
	auto collidedX = expelX > 0 && expelX <= expelY;
	auto collidedY = expelY > 0 && expelY <= expelX;

	//Eliminate motion in direction of collision and reposition outside collision
	if(collidedX) {
		if(expelLeft > 0)
			motion.velocity.x = max(0.0f, motion.velocity.x);
		if(expelRight > 0)
			motion.velocity.x = min(0.0f, motion.velocity.x);

		rect.position.x += expelLeft;
		rect.position.x -= expelRight;
	}

	if(collidedY) {
		if(expelTop > 0)
			motion.velocity.y = min(0.0f, motion.velocity.y);
		if(expelBot > 0)
			motion.velocity.y = max(0.0f, motion.velocity.y);

		rect.position.y -= expelTop;
		rect.position.y += expelBot;
	}
}

void expel(Actor &actor, Actor &hit) {
	auto &bb = hit.get<Collider>().bounds;
	auto &obb = actor.get<Collider>().bounds;

	auto delta = obb.position - bb.position;
	auto alpha = delta.abs().scale(2 / Vector2(bb.size.x, bb.size.y)).magnitude();

	actor.get<Motion>().velocity += delta / (alpha * alpha);
}

void Collider::defaultResponse(Actor &actor, Actor &hit) {
	if(hit.has<Motion>())
		expel(actor, hit);
	else
		clip(actor, hit);
}

Collider::Collider(Vector2 size) : bounds(Vector2::zero, size) {
	this->size = size;
	this->respond = defaultResponse;
}

void Collider::recalculateBounds() {
	this->bounds.position = actor->get<Rect>().position;
	this->bounds.size = size;
}