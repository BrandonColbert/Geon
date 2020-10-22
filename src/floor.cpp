#include "generation/floor.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <tuple>
#include "console.h"
#include "generation/value_noise.h"
#include "structures/grid.h"

using namespace std;
using Tile = Tilemap::Tile;

Floor::Floor(int width, int height, int seed) : width(width), height(height), rand(seed) {
	roomCount = (width + height) / 2;
	randomness = 0;
}

Tilemap Floor::generate() {
	Grid<Tile> tiles(width, height, Tile::None);
	Grid<Tile> newTiles(width, height, Tile::None);

	auto a = ceil(sqrt(roomCount));
	vector<tuple<int, int>> positions;

	for(auto j = 0.0; j < a; j++)
		for(auto i = 0.0; i < a; i++)
			positions.push_back(make_tuple(
				width * (i + 1) / (a + 1),
				height * (j + 1) / (a + 1)
			));

	while((int)positions.size() > roomCount)
		positions.erase(positions.begin() + rand.next<int>(0, positions.size()));

	for(auto p : positions)
		tiles(p) = Tile::Floor;

	auto iteration = 0;
	cout << "i=" << iteration << endl;
	Tilemap(tiles).visualize();

	while(iteration++ < randomness) {
		auto noise = ValueNoise(width, height, rand.next<int>()).toGrid();

		newTiles = Grid<Tile>(width, height, Tile::None);
		
		tiles.forEach([&](int x, int y) {
			if(tiles(x, y) == Tile::None)
				return;

			vector<tuple<int, int>> positions;
			vector<float> values;

			positions.push_back(make_tuple(x, y));
			values.push_back(noise(x, y));

			for(auto n : tiles.getNeighbors(x, y, 1.5)) {
				positions.push_back(n);
				values.push_back(noise(n));
			}

			auto newPosition = positions[max_element(values.begin(), values.end()) - values.begin()];
			
			if(newTiles(newPosition) == 0 && tiles(newPosition) == 0)
				newTiles(newPosition) = tiles(x, y);
			else
				newTiles(x, y) = tiles(x, y);
		});

		tiles = newTiles;

		cout << endl << "i=" << iteration << endl;
		Tilemap(tiles).visualize();
	}

	tiles.forEach([&](int x, int y) {
		if(tiles(x, y) != 1)
			return;

		auto halfLength = rand.next(minRoomSideLength, maxRoomSideLength) / 2;
		auto halfWidth = rand.next(minRoomSideLength, maxRoomSideLength) / 2;

		for(auto j = -halfWidth; j < halfWidth; j++)
			for(auto i = -halfLength; i < halfLength; i++)
				if(tiles(x + i, y + j) == Tile::None)
					newTiles(x + i, y + j) = Tile::Floor;
	});

	tiles = newTiles;

	cout << endl << "i=" << ++iteration << endl;
	Tilemap(tiles).visualize();

	return Tilemap(tiles);
}