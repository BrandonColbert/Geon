#include "components/collider.h"

#include <algorithm>
#include <cmath>
#include "components/rect.h"

using namespace std;

#include "console.h"

void defaultResponse(Actor &actor, Actor &hit) {
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

Collider::Collider(Vector2 size) : bounds(Vector2::zero, size) {
	this->size = size;
	this->respond = defaultResponse;
}

void Collider::recalculateBounds() {
	this->bounds.position = actor->get<Rect>().position;
	this->bounds.size = size;
}