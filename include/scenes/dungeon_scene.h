#pragma once

#include "scenes/scene.h"
#include "generation/tilemap.h"
#include "structures/grid.h"

/**
 * geon that is not Dun
 */
class DungeonScene : public Scene {
	public:
		/**
		 * Tilemap of the current floor
		 */
		static Tilemap tilemap;

		/**
		 * Number of enemies remaining in the level
		 */
		static int remainingEnemies;

		/**
		 * Floor level
		 */
		static int floorLevel;

		/**
		 * Start the game
		 */
		static void play();

		/**
		 * Progress to the next level
		 */
		static void progress();

		/**
		 * Reset to level 1
		 */
		static void reset();
	protected:
		void create() override;
};