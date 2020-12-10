#pragma once

#include <vector>
#include <set>
#include "generation/tilemap.h"
#include "utils/random.h"

class Room {
	public:
		Point center;
		std::set<Point> anchors;
		std::set<Point> region;
};

class FloorInfo {
	public:
		std::vector<Room> rooms;
		Tilemap tilemap;
};

/**
 * Generates a tilemap for the floor of the dungeon based on the specified parameters
 */
class Floor {
	public:
		float distribution = 1;
		float spacing = 1;
		int rooms = 1;
		int minRoomSideLength = 1, maxRoomSideLength = 1;
		int maxIterations = 1000;

		Floor(int width, int height, int seed);
		FloorInfo generate();
	private:
		int width, height;
		Random::Sequence rand;
};