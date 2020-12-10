#include "scenes/dungeon_scene.h"

#include <iterator>
#include "actor.h"
#include "generation/floor.h"
#include "components/rect.h"
#include "structures/grid.h"
#include "structures/color.h"
#include "system.h"
#include "utils/format.h"
#include "utils/random.h"
#include "engine.h"

#include "scenes/dungeon/warrior.h"
#include "scenes/dungeon/slime.h"
#include "scenes/dungeon/wolf.h"
#include "scenes/dungeon/skeleton.h"
#include "scenes/dungeon/health.h"

using namespace std;
using namespace Format;
using Actors = Engine::Actors;
using Systems = Engine::Systems;
using Input = Engine::Input;
using Display = Engine::Display;

#include "components/player.h"
#include "components/focus.h"

Tilemap DungeonScene::tilemap;
int DungeonScene::remainingMobs = 0;

void DungeonScene::create() {
	Display::zoom = 0.8;
	Display::backgroundColor = Color(0.05, 0.05, 0.2);

	//Create the map
	Floor floor(50, 50, 1);
	floor.rooms = 8;
	floor.distribution = 2;
	floor.spacing = 1;
	floor.minRoomSideLength = 10;
	floor.maxRoomSideLength = 20;

	auto info = floor.generate();
	tilemap = info.tilemap;
	tilemap.tileSize = 50;
	tilemap.actualize();
	tilemap.visualize();

	auto mainRoom = info.rooms.front();

	for(auto room : info.rooms)
		if(room.region.size() > mainRoom.region.size())
			mainRoom = room;

	auto &player = Warrior::spawn();
	player.get<Rect>().position = tilemap.tileToWorld(mainRoom.center);

	auto &tiles = tilemap.getTiles();
	auto seq = Random::Sequence(11);
	
	for(auto room : info.rooms) {
		for(auto i = 0; i < 5; i++) {
			Point point;

			do {
				auto index = seq.next(0, (int)room.region.size() - 1);
				point = *next(room.region.begin(), index);
			} while(tiles(point) != Tilemap::Tile::Floor);
			
			Actor *mob;

			switch(seq.next(0, 2)) {
				case 0:
					mob = &Slime::spawn(tilemap.tileToWorld(point));
					break;
				case 1:
					mob = &Wolf::spawn(tilemap.tileToWorld(point));
					break;
				case 2:
					mob = &Skeleton::spawn(tilemap.tileToWorld(point));
					break;
			}

			mob->get<Health>().onDeath = [&](Actor &actor) {
				actor.destroy();
				remainingMobs--;
			};

			remainingMobs++;
		}
	}
}