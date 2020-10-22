#pragma once

#include "structures/grid.h"

/**
 * Used to store the map and create it in the scene
 */
class Tilemap {
	public:
		enum Tile {
			None = 0,
			Floor = 1,
			Wall = 2,
			Path = 3
		};

		float tileSize = 1;

		Tilemap(Grid<Tile> tiles);

		/**
		 * Generates the tilemap in the scene
		 */
		void actualize();

		/**
		 * Displays a visual of the tilemap in the console
		 */
		void visualize();
	private:
		Grid<Tile> tiles;
};