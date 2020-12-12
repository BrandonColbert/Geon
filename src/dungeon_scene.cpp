#include "scenes/dungeon_scene.h"

#include <exception>
#include <string>
#include <fstream>
#include <map>
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
#include "scenes/dungeon/menu_system.h"

using namespace std;
using namespace Format;
using Actors = Engine::Actors;
using Systems = Engine::Systems;
using Input = Engine::Input;
using Display = Engine::Display;

#include "components/player.h"
#include "components/focus.h"
using Tile = Tilemap::Tile;

Tilemap DungeonScene::tilemap;
int DungeonScene::remainingEnemies = 0;
int DungeonScene::floorLevel = 0;

int floorLevel = 0;

void DungeonScene::create() {
	Systems::add<MenuSystem>();

	Display::zoom = 0.8;
	Display::backgroundColor = Color(0.05, 0.05, 0.2);
}

void DungeonScene::play() {
	//Load floor parameters
	map<string, float> params{
		{"seed", 1},
		{"width", 50},
		{"height", 50},
		{"rooms", 8},
		{"distribution", 2},
		{"spacing", 1},
		{"min_room_side_length", 10},
		{"max_room_side_length", 20},
		{"enemies_per_room", 5}
	};

	const auto configPath = "floor.txt";
	ifstream config(configPath);

	if(config.good()) {
		Console::print("Reading config...");

		string line;

		while(getline(config, line)) {
			if(line.rfind("#", 0) != string::npos)
				continue;

			auto equalPos = line.find('=');

			if(equalPos == string::npos)
				continue;

			auto key = line.substr(0, equalPos);
			auto value = line.substr(equalPos + 1, line.size() - equalPos - 1);

			if(params.find(key) == params.end()) {
				Console::error("\t'%' is not a configurable parameter for floor generation", key);
				continue;
			}

			try {
				params[key] = stof(value);
				Console::print("\tUsing % for '%'", params[key], key);
			} catch(invalid_argument) {
				Console::error("\tAttempted to use non-numeric value '%' for '%', defaulting to %", value, key, params[key]);
			}
		}

		Console::print("");
	} else {
		Console::error("Config at '%' missing! Using default floor generation values", configPath);
		Console::print("");
	}

	//Create the map
	Floor floor(params["width"], params["height"], (int)params["seed"] + floorLevel);
	floor.rooms = params["rooms"];
	floor.distribution = params["distribution"];
	floor.spacing = params["spacing"];
	floor.minRoomSideLength = params["min_room_side_length"];
	floor.maxRoomSideLength = params["max_room_side_length"];

	auto info = floor.generate();
	tilemap = info.tilemap;
	tilemap.tileSize = 50;
	tilemap.actualize();
	// tilemap.visualize();

	auto mainRoom = info.rooms.front();

	for(auto room : info.rooms)
		if(room.region.size() > mainRoom.region.size())
			mainRoom = room;

	auto &player = Warrior::spawn(Vector2::right * 80);
	player.get<Rect>().position = tilemap.tileToWorld(mainRoom.center);

	player.get<Health>().onDeath = [](Actor &actor) {
		actor.destroy();
		MenuSystem::currentMenu = Menu::Death;
	};

	auto &tiles = tilemap.getTiles();
	auto seq = Random::Sequence(10 + (int)params["seed"] + floorLevel);
	
	for(auto room : info.rooms) {
		for(auto i = 0; i < params["enemies_per_room"]; i++) {
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

				remainingEnemies--;

				if(remainingEnemies > 0)
					return;

				progress();
			};

			remainingEnemies++;
		}
	}
}

void DungeonScene::progress() {
	Actors::forEach([](Actor &actor) {
		actor.destroy();
	});

	floorLevel++;
	remainingEnemies = 0;
	DungeonScene::play();
}

void DungeonScene::reset() {
	Actors::forEach([](Actor &actor) {
		actor.destroy();
	});

	remainingEnemies = 0;
	floorLevel = 0;
}