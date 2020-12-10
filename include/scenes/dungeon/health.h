#pragma once

#include <functional>
#include "component.h"

/**
 * Represents a destructable actor's health
 */
class Health : public Component {
	public:
		/**
		 * Called when this actor's health reaches zero or kill has been called
		 * 
		 * Destroys the actor by default
		 */
		std::function<void(Actor&)> onDeath;

		/**
		 * Called when the actor's health changes
		 * 
		 * Passes the actor, the old health, and the new health
		 */
		std::function<void(Actor&, float, float)> onChange;

		/**
		 * Amount of time this entity is invincible for after getting hit
		 */
		float invincibilityDuration = 0.0f;

		/**
		 * @param value Maximum health of the actor
		 */
		Health(float value);

		/**
		 * @return Actor's current health 
		 */
		float current();

		/**
		 * @return Actor's max health 
		 */
		float max();

		/**
		 * Damages the actor
		 * @param amount Damage to deal
		 */
		void damage(float amount);

		/**
		 * Heals the actor
		 * @param amount How much to heal
		 */
		void heal(float amount);

		/**
		 * Kills the actor regardless of its remaining health
		 */
		void kill();
	private:
		float value, maxValue;
		float lastHitTime = 0.0f;
};