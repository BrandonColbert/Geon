#pragma once

#include "structures/grid.h"
#include "structures/vector2.h"
#include "structures/point.h"

/**
 * Used to store the map and create it in the scene
 */
class Tilemap {
	public:
		enum Tile {
			None, //The void
			Floor, //Floorboard
			Wall, //Wall
			Path, //Path to another room
			Plank //Destructable plank
		};

		float tileSize = 1;

		Tilemap(Grid<Tile> tiles);

		Tilemap();

		Vector2 tileToWorld(Point);

		Point worldToTile(Vector2);

		/**
		 * Generates the tilemap in the scene
		 */
		void actualize();

		/**
		 * Displays a visual of the tilemap in the console
		 */
		void visualize();

		Grid<Tile>& getTiles();
	private:
		Grid<Tile> tiles;
};