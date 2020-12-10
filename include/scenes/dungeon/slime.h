#pragma once

#include "actor.h"
#include "structures/vector2.h"

class Slime {
	public:
		static Actor& spawn(Vector2 position = Vector2::zero);
};