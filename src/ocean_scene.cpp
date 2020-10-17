#include "scenes/ocean_scene.h"

#include <utility>
#include "component.h"
#include "components/animator.h"
#include "components/controller.h"
#include "components/focus.h"
#include "components/motion.h"
#include "components/player.h"
#include "components/rect.h"
#include "components/spawner.h"
#include "components/sprite.h"
#include "engine.h"
#include "system.h"
#include "math/vector2.h"
#include "utils/random.h"

using std::get;
using Actors = Engine::Actors;
using Display = Engine::Display;
using Systems = Engine::Systems;
using Time = Engine::Time;

class Bubble : public Component {};

class BubbleSystem : public System {
	public:
		void update() override {
			auto dimensions = Display::getScreenSize();
			auto height = get<1>(dimensions);

			Actors::forEach<Bubble, Rect>([&](Actor &actor) {
				auto &rect = actor.get<Rect>();
				rect.position.y += 100 * Time::deltaTime;

				if(rect.position.y > height / 2 + rect.size.y / 2)
					actor.destroy();
			});
		}
};

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

float nextSpawnTime = 0.0f;

void OceanScene::create() {
	Systems::add<BubbleSystem>();
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
	auto &bubbleSpawner = Actor::spawn("Bubble Spawner");
	bubbleSpawner.add(new Spawner());

	auto &spawner = bubbleSpawner.get<Spawner>();

	spawner.spawn = []() -> Actor& {
		auto dimensions = Display::getScreenSize();
		auto width = get<0>(dimensions);
		auto height = get<1>(dimensions);

		auto &bubble = Actor::spawn("Bubble");
		auto size = Vector2(400, 400) * Random::range(0.1f, 0.2f);
		auto pos = Vector2(Random::range(-width / 2, width / 2), -height / 2 - size.y);
		bubble.add(new Rect(pos, size));
		bubble.add(new Sprite(Texture("./resources/bubble.png")));
		bubble.add(new Bubble());

		nextSpawnTime = Time::time + Random::range(0.5f, 1.0f);

		return bubble;
	};

	spawner.condition = []() -> bool {
		return Time::time >= nextSpawnTime;
	};
}