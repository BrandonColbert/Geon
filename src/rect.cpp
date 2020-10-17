#include "components/rect.h"

Rect::Rect(Vector2 position, Vector2 size) {
	this->position = position;
	this->size = size;
}

bool Rect::isOverlapping(const Rect &other) const {
	if(position.x + size.x / 2 < other.position.x - other.size.x / 2)
		return false;
	if(position.x - size.x / 2 > other.position.x + other.size.x / 2)
		return false;
	if(position.y + size.y / 2 < other.position.y - other.size.y / 2)
		return false;
	if(position.y - size.y / 2 > other.position.y + other.size.y / 2)
		return false;

	return true;
}