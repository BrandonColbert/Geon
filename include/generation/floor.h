#pragma once

#include <vector>
#include "generation/tilemap.h"
#include "utils/random.h"

/**
 * Generates a tilemap for the floor of the dungeon based on the specified parameters
 */
class Floor {
	public:
		int randomness;
		int roomCount;
		int minRoomSideLength, maxRoomSideLength;

		Floor(int width, int height, int seed);
		Tilemap generate();
	private:
		int width, height;
		Random::Sequence rand;
};