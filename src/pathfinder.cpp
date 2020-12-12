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

Pathfinder::Pathfinder(Tilemap &tilemap, float sight) : tilemap(tilemap) {
	this->sight = sight;
}

bool Pathfinder::chase(Actor &target) {
	auto position = getActor().get<Rect>().position;
	auto targetPosition = target.get<Rect>().position;
	auto distance = (targetPosition - position).magnitude();

	if(distance <= sight) {
		if(chasing)
			switch(moveTo(targetPosition)) {
				case Unreachable:
					chasing = false;
					canSearch = false;
					break;
				default:
					canSearch = true;
					lastKnownTargetPosition = targetPosition;
					break;
			}
		else if(hasLOS(targetPosition))
			chasing = true;
	} else if(canSearch)
		switch(moveTo(lastKnownTargetPosition)) {
			case Unreachable:
			case Reached:
				canSearch = false;
				break;
			default:
				break;
		}			

	return chasing;
}

PathResult Pathfinder::moveTo(Vector2 point) {
	auto &tilemap = DungeonScene::tilemap;
	auto &controller = getActor().get<Controller>();
	auto position = getActor().get<Rect>().position;

	if(tilemap.worldToTile(position) == tilemap.worldToTile(point)) {
		path.clear();
		controller.move = Vector2::zero;

		return PathResult::Reached;
	}

	if(hasLOS(point)) {
		path.clear();
		controller.move = (point - position).normalized();
	} else if(path.size() > 0) {
		auto nextPosition = tilemap.tileToWorld(path[0]);
		controller.move = (nextPosition - position).normalized();

		if(tilemap.worldToTile(position) == path[0])
			path.erase(path.begin());
	} else {
		auto &tiles = tilemap.getTiles();
		auto start = tilemap.worldToTile(position);
		auto end = tilemap.worldToTile(point);

		path = Search::astar(tiles, start, end, [&](Point p) {
			switch(tiles(p)) {
				case Tile::Floor:
				case Tile::Path:
					return true;
				default:
					return p == start || p == end;
			}
		});

		if(path.size() == 0)
			return PathResult::Unreachable;

		path.erase(path.begin());

		// Console::print("% [%] -> % [%] | %, %", start, tiles(start), end, tiles(end), path.size(), (point - position).normalized());

		// if(path.size() > 0) {
		// 	cout << "\t" << path[0];

		// 	for(auto i = 1; i < (int)path.size(); i++)
		// 		cout << " " << path[i];

		// 	cout << endl;
		// }
	}

	return PathResult::Reachable;
}

bool Pathfinder::hasLOS(Vector2 point) {
	auto &tiles = tilemap.getTiles();

	auto start = tilemap.worldToTile(actor->get<Rect>().position);
	auto delta = point - actor->get<Rect>().position;
	auto direction = delta.normalized();
	auto distance = delta.magnitude() / tilemap.tileSize;

	return !tiles.raycast(start, direction, nullptr, [&](Point p) {
		switch(tiles(p)) {
			case Tile::Floor:
			case Tile::Path:
				return false;
			default:
				return true;
		}
	}, distance);
}