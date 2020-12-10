#include "generation/floor.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
#include "console.h"
#include "structures/grid.h"
#include "utils/search.h"

using namespace std;
using Tile = Tilemap::Tile;

template<typename T>
void visualize(Grid<T> tiles) {
	cout << tiles.toString([](T v) {
		stringstream stream;
		stream << fixed << setprecision(2);
		
		if(v == 0)
			stream << " " << T(0);
		else {
			if(!signbit(v))
				stream << " ";

			stream << v;
		}

		return stream.str();
	});
}

Floor::Floor(int width, int height, int seed) : width(width), height(height), rand(seed) {}

set<Point> captureRoom(Grid<Tile> tiles, Point point) {
	set<Point> open{point}, close;

	while(open.size() > 0) {
		auto p = *open.begin();
		open.erase(p);
		close.emplace(p);

		for(auto n : tiles.getNeighbors(p)) {
			switch(tiles(n)) {
				case Tile::None:
					break;
				case Tile::Wall:
					close.emplace(n);
					break;
				default:
					if(close.find(n) == close.end())
						open.emplace(n);
					break;
			}
		}
	}

	return close;
}

//TODO: Show generation messages in UI

FloorInfo Floor::generate() {
	Grid<float> regions(width, height, 1);

	auto count = rooms;
	auto iteration = 0;

	//Create room points
	Console::print("Picking room points...");
	while(count > 0 && iteration < maxIterations) {
		iteration++;

		//Pick potential room space
		auto x = rand.next(0, width - 1);
		auto y = rand.next(0, height - 1);

		//Determine whether to create a room based on space probability value
		if(rand.next(0.0f, 1.0f) > regions(x, y))
			continue;

		//Adjust probabilities for neighboring spaces
		regions(x, y) = -1;
		for(auto n : regions.getNeighbors(x, y, distribution)) {
			if(regions(n) < 0)
				continue;

			regions(n) *= pow((Vector2(x, y) - n).magnitude() / (float)distribution, spacing);
		}

		count--;
	}

	if(iteration >= maxIterations)
		Console::log("Floor generation reached max iterations (%)", maxIterations);

	//Convert space probabilities into tilemap
	Grid<Tile> tiles(width, height, [&](int x, int y) {
		if(regions(x, y) == -1)
			return Tile::Floor;

		return Tile::None;
	});

	//Create rooms with random sizes
	Console::print("Creating rooms...");
	tiles.forEach([&](int x, int y) {
		if(regions(x, y) != -1)
			return;

		auto halfLength = rand.next(minRoomSideLength, maxRoomSideLength) / 2;
		auto halfWidth = rand.next(minRoomSideLength, maxRoomSideLength) / 2;

		for(auto j = -halfWidth; j <= halfWidth; j++) {
			auto ny = y + j;

			for(auto i = -halfLength; i <= halfLength; i++) {
				auto nx = x + i;

				if(!tiles.inside(nx, ny))
					continue;

				switch(tiles(nx, ny)) {
					case Tile::None:
						if(nx == x - halfLength || nx == x + halfLength)
							tiles(nx, ny) = Tile::Wall;
						else if(ny == y - halfWidth || ny == y + halfWidth)
							tiles(nx, ny) = Tile::Wall;
						else
							tiles(nx, ny) = Tile::Floor;
						break;
					case Tile::Wall:
						tiles(nx, ny) = Tile::Floor;
						break;
					default:
						break;
				}
			}
		}
	});

	//Seal edges
	Console::print("Sealing rooms...");
	tiles.forEach([&](int x, int y) {
		switch(tiles(x, y)) {
			case Tile::Floor:
			case Tile::Path:
				if(tiles.getNeighbors(x, y).size() == 4)
					break;

				tiles(x, y) = Tile::Wall;
				break;
			default:
				break;
		}
	});

	//Connect rooms
	Console::print("Connecting rooms...");
	vector<Room> rooms;

	regions.forEach([&](int x, int y) {
		if(regions(x, y) != -1)
			return;

		auto anchor = Point(x, y);

		Room room;
		room.region = captureRoom(tiles, anchor);

		for(auto p : room.region)
			room.center += p;

		room.center = Vector2(room.center) / room.region.size();

		for(auto other : rooms) {
			if(other.center == room.center) {
				other.anchors.emplace(anchor);
				return;
			}
		}

		room.anchors.emplace(anchor);
		rooms.push_back(room);
	});

	for(auto i = 1; i < (int)rooms.size(); i++) {
		auto start = rooms[i - 1];
		auto end = rooms[i];

		auto path = Search::astar(tiles, start.center, end.center, [&](Point p) {
			if(p.x == 0 || p.x == tiles.getWidth() - 1)
				return false;
			if(p.y == 0 || p.y == tiles.getHeight() - 1)
				return false;

			switch(tiles(p)) {
				case Tile::None:
				case Tile::Path:
					return true;
				default:
					break;
			}

			return start.region.find(p) != start.region.end() || end.region.find(p) != end.region.end();
		});

		for(auto point : path)
			tiles(point) = Tile::Path;
	}

	//Add walls around connectors
	Console::print("Sealing connections...");
	tiles.forEach([&](int x, int y) {
		if(tiles(x, y) != Tile::Path)
			return;

		for(auto n : tiles.getNeighbors(x, y)) {
			if(tiles(n) != Tile::None)
				continue;

			tiles(n) = Tile::Wall;
		}
	});

	Console::print("Tidying rooms...");

	//Convert paths inside rooms to floors
	for(auto room : rooms)
		for(auto point : room.region)
			if(tiles(point) == Tile::Path)
				tiles(point) = Tile::Floor;

	//Patch holes in rooms
	tiles.update([&](int x, int y) {
		if(tiles(x, y) == Tile::None)
			for(auto n : tiles.getNeighbors(x, y))
				if(tiles(n) == Tile::Floor)
					return Tile::Wall;

		return tiles(x, y);
	});

	FloorInfo info;
	info.tilemap = Tilemap(tiles);
	info.rooms = rooms;

	return info;
}