#include "structures/bounding_box.h"

BoundingBox::BoundingBox(Vector2 position, Vector2 size) {
	this->position = position;
	this->size = size;
}

bool BoundingBox::overlaps(const BoundingBox &other) const {
	if(position.x + size.x / 2 <= other.position.x - other.size.x / 2)
		return false;
	if(position.x - size.x / 2 >= other.position.x + other.size.x / 2)
		return false;
	if(position.y + size.y / 2 <= other.position.y - other.size.y / 2)
		return false;
	if(position.y - size.y / 2 >= other.position.y + other.size.y / 2)
		return false;

	return true;
}

Vector2 BoundingBox::expulsion(const BoundingBox &other) const {
	auto delta = other.position - position;
	auto alpha = delta.abs().scale(Vector2(2 / size.x, 2 / size.y)).magnitude();

	return delta / (alpha * alpha);
}