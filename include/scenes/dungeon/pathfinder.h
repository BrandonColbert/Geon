#pragma once

#include <vector>
#include "component.h"
#include "structures/vector2.h"

class Pathfinder : public Component {
	public:
		std::vector<Point> path;
		bool moveTo(Actor &target);
		bool hasLOS(Actor &target);
};