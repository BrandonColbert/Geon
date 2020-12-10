#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include "actor.h"
#include "system.h"

class MobSystem : public System {
	public:
		using Behavior = std::function<void(Actor&, int)>;

		/**
		 * How many ticks occur per second
		 */
		static int tickRate;

		void update() override;

		/**
		 * Sets the behavior for a mob type
		 * @param type Mob type
		 * @param behavior Updates the logic of the mob, passing the update tick and the mob
		 */
		static void setBehavior(std::string type, Behavior behavior);

		/**
		 * Deletes the behavior of a mob type
		 * @param type Mob type
		 */
		static void deleteBehavior(std::string type);

		/**
		 * @param type Mob type
		 * @return Whether the mob type exists
		 */
		static bool typeExists(std::string type);
	private:
		static std::unordered_map<std::string, Behavior> behaviors;
};