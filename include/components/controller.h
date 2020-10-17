#pragma once

#include "component.h"
#include "math/vector2.h"

class Controller : public Component {
	public:
		float speed;
		Vector2 move, lastMove;
		Controller(float speed);
};