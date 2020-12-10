#include "scenes/dungeon/pathfinder.h"

#include "components/rect.h"
#include "components/controller.h"
#include "structures/point.h"
#include "utils/search.h"

#include "scenes/dungeon_scene.h"
#include "generation/tilemap.h"

#include "console.h"

using namespace std;
using Tile = Tilemap::Tile;

bool Pathfinder::moveTo(Actor &target) {
	auto &controller = actor->get<Controller>();
	auto &rect = actor->get<Rect>();

	if(hasLOS(target)) {
		// Console::print("LOSing");
		controller.move = (target.get<Rect>().position - rect.position).normalized();
		return true;
	}

	auto &tilemap = DungeonScene::tilemap;
	auto &tiles = tilemap.getTiles();

	if(path.size() == 0) {
		auto start = tilemap.worldToTile(rect.position);
		auto end = tilemap.worldToTile(target.get<Rect>().position);

		path = Search::astar(tiles, start, end, [&](Point p) {
			return tiles(p) == Tile::Floor || p == start || p == end;
		});

		if(path.size() == 0)
			return false;
	}

	// Console::print("% -> % | %, %", start, end, path.size(), direction);

	// cout << "\t" << path[0];
	// for(auto i = 1; i < (int)path.size(); i++)
	// 	cout << " " << path[i];
	// cout << endl;

	if(tilemap.worldToTile(rect.position) == path[0]) {
		path.erase(path.begin());
		return true;
	}

	// Console::print("Pathing");
	controller.move = (tilemap.tileToWorld(path[0]) - rect.position).normalized();

	return true;
}

bool Pathfinder::hasLOS(Actor &target) {
	auto &tilemap = DungeonScene::tilemap;
	auto &tiles = tilemap.getTiles();

	auto delta = target.get<Rect>().position - actor->get<Rect>().position;

	return !tiles.raycast(tilemap.worldToTile(actor->get<Rect>().position), delta.normalized(), nullptr, [&](Point p) {
		switch(tiles(p)) {
			case Tile::Floor:
			case Tile::Path:
				return false;
			default:
				return true;
		}
	}, delta.magnitude() / tilemap.tileSize);
}