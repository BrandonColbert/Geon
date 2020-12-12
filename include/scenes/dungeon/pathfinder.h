#pragma once

#include <vector>
#include "component.h"
#include "structures/vector2.h"
#include "generation/tilemap.h"

enum PathResult {
	Unreachable,
	Reachable,
	Reached
};

/**
 * Enables a controllable actor to pathfind towards another actor
 * 
 * Dependent on the actor having a Rect and Controllable component
 */
class Pathfinder : public Component {
	public:
		/** How far away this entity can see an actor from */
		float sight;

		/**
		 * @param tilemap Map the entity is located in
		 */
		Pathfinder(Tilemap &tilemap, float sight);

		/**
		 * Chases after the specified actor if possible
		 * @param target Actor to chase
		 * @returns Whether the target is being chased
		 */
		bool chase(Actor &target);

		/**
		 * Moves towards the the target point
		 * @param point Target point
		 * @returns Result of pathing
		 */
		PathResult moveTo(Vector2 point);

		/**
		 * @param point Point to determine if directly visible
		 * @return Whether this actor has line of sight to the point
		 */
		bool hasLOS(Vector2 point);
	private:
		std::vector<Point> path;
		Tilemap &tilemap;

		bool chasing = false, canSearch = false;
		Vector2 lastKnownTargetPosition;
};