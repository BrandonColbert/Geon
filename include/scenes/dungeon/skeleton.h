#pragma once

#include "actor.h"
#include "structures/vector2.h"

class Skeleton {
	public:
		static Actor& spawn(Vector2 position = Vector2::zero);
};