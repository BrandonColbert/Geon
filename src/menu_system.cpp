#include "scenes/dungeon/menu_system.h"

#include <algorithm>
#include <cmath>
#include <sstream>
#include "engine.h"
#include "systems/overlay_system.h"
#include "structures/vector2.h"
#include "structures/color.h"
#include "utils/format.h"

#include "scenes/dungeon_scene.h"

#include "console.h"

using namespace std;
using Input = Engine::Input;
using Display = Engine::Display;

int pauseIndex = 0, mainIndex = 0, deathIndex = 0;
Menu MenuSystem::currentMenu = Menu::Main;
bool justPaused = false;

void MenuSystem::update() {
	switch(currentMenu) {
		case Ingame: {
			//Level
			stringstream level;
			Format::formatTo(level, "Level %", 1 + DungeonScene::floorLevel);

			OverlaySystem::text(level.str(), Vector2(-0.25, 0.9), Color::white, 0.75);

			//Remaining
			stringstream remaining;
			Format::formatTo(remaining, "Enemies: %", DungeonScene::remainingEnemies);

			OverlaySystem::text(remaining.str(), Vector2(0.25, 0.9), Color::white, 0.5);

			//Pausing
			if(Input::down("escape")) {
				Engine::getInstance().paused = true;
				currentMenu = Menu::Pause;
				justPaused = true;
				return;
			}

			break;
		}
		case Death: {
			auto dim = Display::getScreenSize();

			OverlaySystem::box(Vector2::zero, dim, Color(0.1, 0.1, 0.1, 0.5));

			OverlaySystem::text("You Died", Vector2::up * 0.2, Color::white, 2);
			OverlaySystem::text("Try again?", Vector2::down * 0.25, deathIndex == 0 ? Color::blue : Color::white, 0.75);
			OverlaySystem::text("Quit", Vector2::down * 0.45, deathIndex == 1 ? Color::blue : Color::white, 0.75);

			if(Input::down("w") || Input::down("UpArrow"))
				deathIndex = max(0, deathIndex - 1);
			if(Input::down("s") || Input::down("DownArrow"))
				deathIndex = min(deathIndex + 1, 1);

			if(Input::down("space")) {
				switch(deathIndex) {
					case 0:
						currentMenu = Menu::Ingame;
						DungeonScene::reset();
						DungeonScene::play();
						return;
					case 1:
						exit(1);
						break;
				}
			}

			break;
		}
		default:
			break;
	}
}

void MenuSystem::render() {
	auto &engine = Engine::getInstance();
	auto dim = Display::getScreenSize();

	switch(currentMenu) {
		case Main: {
			OverlaySystem::box(Vector2::zero, dim, Display::backgroundColor);

			OverlaySystem::text("Geon", Vector2::up * 0.8, Color::white, 2);
			OverlaySystem::text("Play", Vector2::zero, mainIndex == 0 ? Color::blue : Color::white);
			OverlaySystem::text("Exit", Vector2::down * 0.25, mainIndex == 1 ? Color::blue : Color::white);

			if(Input::down("w") || Input::down("UpArrow"))
				mainIndex = max(0, mainIndex - 1);
			if(Input::down("s") || Input::down("DownArrow"))
				mainIndex = min(mainIndex + 1, 1);

			if(Input::down("space")) {
				switch(mainIndex) {
					case 0:
						currentMenu = Menu::Ingame;
						DungeonScene::play();
						return;
					case 1:
						exit(1);
						break;
				}
			}

			break;
		}
		case Pause: {
			if(!justPaused && Input::down("escape")) {
				currentMenu = Menu::Ingame;
				engine.paused = false;
				return;
			} else
				justPaused = false;

			OverlaySystem::box(Vector2::zero, dim, Color(0.1, 0.1, 0.1, 0.5));

			OverlaySystem::text("PAUSED", Vector2::up * 0.8, Color::white, 1.25);
			OverlaySystem::text("Resume", Vector2::up * 0.25, pauseIndex == 0 ? Color::blue : Color::white);
			OverlaySystem::text("Restart", Vector2::zero, pauseIndex == 1 ? Color::blue : Color::white);
			OverlaySystem::text("Next Level", Vector2::down * 0.25, pauseIndex == 2 ? Color::blue : Color::white);
			OverlaySystem::text("Exit", Vector2::down * 0.5, pauseIndex == 3 ? Color::blue : Color::white);

			OverlaySystem::text("Move: WASD", Vector2(0.8, 0.75), Color::white, 0.5);
			OverlaySystem::text("Shoot: Space", Vector2(0.8, 0.65), Color::white, 0.5);
			OverlaySystem::text("Zoom: -/+", Vector2(0.8, 0.55), Color::white, 0.5);
			OverlaySystem::text("Reset Zoom: =", Vector2(0.8, 0.45), Color::white, 0.5);

			if(Input::down("w") || Input::down("UpArrow"))
				pauseIndex = max(0, pauseIndex - 1);
			if(Input::down("s") || Input::down("DownArrow"))
				pauseIndex = min(pauseIndex + 1, 3);

			if(Input::down("space")) {
				switch(pauseIndex) {
					case 0:
						currentMenu = Menu::Ingame;
						engine.paused = false;
						return;
					case 1:
						currentMenu = Menu::Ingame;
						DungeonScene::reset();
						DungeonScene::play();
						engine.paused = false;
						break;
					case 2:
						currentMenu = Menu::Ingame;
						DungeonScene::progress();
						engine.paused = false;
						break;
					case 3:
						currentMenu = Menu::Main;
						DungeonScene::reset();
						engine.paused = false;
						return;
				}
			}

			break;
		}
		default:
			break;
	}

	switch(currentMenu) {
		case Ingame:
			break;
		default:
			OverlaySystem::text(
				"Use W/D or Up/Down Arrow to choose option and Spacebar to select",
				Vector2::down * 0.9,
				Color::white,
				0.3
			);
			break;
	}
}