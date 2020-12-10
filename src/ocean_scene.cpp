#include "scenes/ocean_scene.h"

#include <utility>
#include "component.h"
#include "components/animator.h"
#include "components/controller.h"
#include "components/focus.h"
#include "components/motion.h"
#include "components/player.h"
#include "components/rect.h"
#include "components/emitter.h"
#include "components/sprite.h"
#include "engine.h"
#include "structures/vector2.h"
#include "system.h"
#include "utils/random.h"

using Actors = Engine::Actors;
using Display = Engine::Display;
using Systems = Engine::Systems;
using Time = Engine::Time;

class SubmarineSystem : public System {
	void update() override {
		Actors::forEach<Player, Controller, Sprite>([](Actor &actor) {
			auto &controller = actor.get<Controller>();
			auto &sprite = actor.get<Sprite>();

			if(controller.move.x != 0 && controller.move != controller.lastMove)
				sprite.flipX = controller.move.x < 0;
		});
	}
};

void OceanScene::create() {
	Systems::add<SubmarineSystem>();

	//Create underwater background
	auto &background = Actor::spawn("Background");
	background.add(new Sprite(Texture("./resources/background.jpg"), -1));
	background.add(new Rect(Vector2(0, 0), Vector2(626, 396) * 1.25));

	//Create controllable submarine
	auto &submarine = Actor::spawn("Submarine");
	submarine.add(new Sprite(Texture("./resources/submarine.png"), 1));
	submarine.add(new Rect(Vector2(0, 0), Vector2(420, 275) * 0.3));
	submarine.add(new Controller(200));
	submarine.add(new Player());

	//Create bubble spawner
	auto &bubbles = Actor::spawn("Bubble Spawner");

	//Setup bubble emitter
	auto &emitter = bubbles.add<Emitter>();
	emitter.frequency = 1.0 / 0.75;
	emitter.emitSize = []() { return Vector2(400, 400) * Random::range(0.1f, 0.2f); };
	emitter.emitVelocity = []() { return Vector2(0, 50); };
	emitter.emitTexture = []() { return Texture("./resources/bubble.png"); };

	emitter.emitPosition = []() {
		auto dim = Display::getScreenSize();
		return Vector2(Random::range(-dim.x / 2, dim.x / 2), -dim.y / 2 - 80);
	};

	emitter.decay = [](Actor &actor, float lifespan, float lifetime) {
		auto dim = Display::getScreenSize();

		auto &rect = actor.get<Rect>();
		rect.position.y += 100 * Time::deltaTime;

		return rect.position.y > dim.y / 2 + rect.size.y / 2;
	};
}