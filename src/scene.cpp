#include "scenes/scene.h"

#include <string>
#include "console.h"
#include "engine.h"
#include "systems/animation_system.h"
#include "systems/collision_system.h"
#include "systems/control_system.h"
#include "systems/focus_system.h"
#include "systems/lifespan_system.h"
#include "systems/motion_system.h"
#include "systems/player_system.h"
#include "systems/spawner_system.h"
#include "systems/sprite_system.h"

using std::string, std::get;
using Systems = Engine::Systems;
using Time = Engine::Time;

const string Name = "Game";
const int Width = 640;
const int Height = 480;
const int Fps = 60;

void Scene::start() {
	Console::print("Starting % at %x% %fps...", Name, Width, Height, Fps);

	auto &engine = Engine::getInstance();
	engine.setupDisplay(Name, Width, Height);
	engine.running = true;

	Time::targetFps = Fps;

	Systems::add<SpawnerSystem>();
	Systems::add<ControlSystem>();
	Systems::add<PlayerSystem>();
	Systems::add<CollisionSystem>();
	Systems::add<MotionSystem>();
	Systems::add<LifespanSystem>();
	Systems::add<FocusSystem>();
	Systems::add<AnimationSystem>();
	Systems::add<SpriteSystem>();

	create();

	while(engine.running) {
		engine.handleEvents();
		engine.update();
		engine.render();
		engine.clock();
	}
}