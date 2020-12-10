#pragma once

#include <functional>
#include "actor.h"
#include "component.h"
#include "components/motion.h"
#include "structures/bounding_box.h"
#include "structures/vector2.h"

using Response = std::function<void(Actor&, Actor&)>;

/**
 * Allows an object to be collided with
 */
class Collider : public Component {
	public:
		/** Collision region size */
		Vector2 size;

		/** Whether the collider is currently collided or not */
		bool isCollided = false;

		/** Bounding box for this collider */
		BoundingBox bounds;

		/**
		 * Collision response called by collision system
		 * 
		 * Arguments are this collider's actor and the hit collider's actor
		 * 
		 * This collider will always have a Motion component
		 */
		Response respond;

		/**
		 * @param size Collision region size
		 */
		Collider(Vector2 size);

		/**
		 * Updates the bounds
		 * 
		 * Actor must have a Rect component for this to work
		 */
		void recalculateBounds();

		static void defaultResponse(Actor &actor, Actor &hit);
};