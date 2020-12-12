#include "scenes/dungeon/wolf.h"

#include <mutex>
#include <string>
#include "component.h"
#include "components/rect.h"
#include "components/controller.h"
#include "components/collider.h"
#include "components/sprite.h"
#include "components/animator.h"
#include "components/player.h"
#include "engine.h"
#include "structures/texture.h"
#include "structures/state.h"
#include "structures/point.h"
#include "utils/timer.h"
#include "utils/moth.h"
#include "utils/random.h"
#include "utils/search.h"

#include "components/mob.h"
#include "systems/mob_system.h"
#include "scenes/dungeon/health.h"
#include "scenes/dungeon/pathfinder.h"
#include "scenes/dungeon_scene.h"

using namespace std;
using namespace Moth;
using Actors = Engine::Actors;

once_flag wolfFlag;

Actor& Wolf::spawn(Vector2 position) {
	call_once(wolfFlag, []() {
		MobSystem::setBehavior("wolf", [](Actor &wolf, int tick) {
			auto &wolfRect = wolf.get<Rect>();
			auto &controller = wolf.get<Controller>();
			controller.move = Vector2::zero;

			Actor *closestPlayer = nullptr;
			float distance;

			Actors::forEach<Player>([&](Actor &actor) {
				if(closestPlayer == nullptr) {
					closestPlayer = &actor;
					distance = (wolfRect.position - actor.get<Rect>().position).magnitude();
				} else {
					auto otherDistance = (wolfRect.position - actor.get<Rect>().position).magnitude();

					if(otherDistance < distance) {
						closestPlayer = &actor;
						distance = otherDistance;
					}
				}
			});

			if(closestPlayer == nullptr)
				return;

			auto &target = *closestPlayer;
			wolf.get<Pathfinder>().chase(target);
		});
	});

	//Create the player
	auto &wolf = Actor::spawn("wolf");
	wolf.add(new Rect(position, Vector2::one * 60));
	wolf.add(new Controller(9));
	wolf.add(new Mob("wolf"));
	wolf.add(new Pathfinder(DungeonScene::tilemap, 600));

	//Health aspects
	auto &health = wolf.add(new Health(6));
	health.onChange = [](Actor &actor, float oldValue, float newValue) {
		if(newValue >= oldValue)
			return;

		actor.get<Sprite>().tint = Color(1, 0.5, 0.5);

		Timer::after(0.25, [&]() {
			if(!actor.exists())
				return;

			actor.get<Sprite>().tint = Color::white;
		});
	};

	//Attack player on collider
	auto &collider = wolf.add(new Collider(Vector2(60, 35)));
	collider.respond = [](Actor &actor, Actor &hit) {
		Collider::defaultResponse(actor, hit);

		if(!hit.has<Player>())
			return;

		hit.get<Health>().damage(1);
	};

	//Add movement constraints
	auto &motion = wolf.add<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 175;
	motion.friction = 0.05;

	//Describe sprite sheet
	auto &sprite = wolf.add(new Sprite(Texture("./resources/mobs/wolf.png"), 1));
	sprite.size = Vector2::one * 16;
	sprite.offset = Vector2::up * 12;

	//Define animation state machine
	auto &animator = wolf.add(new Animator([](Actor &actor, string currentState) {
		auto &motion = actor.get<Motion>();

		if(motion.velocity.sqrMagnitude() > 1)
			return motion.velocity.x < 0 ? "move_left" : "move_right";
		else {
			if(currentState == "move_left")
				return "idle_left";

			if(currentState == "move_right")
				return "idle_right";
		}

		return currentState.c_str();
	}));

	//Describe animation states
	animator.defineState(State("idle_left", 0, {Point(0, 1)}));
	animator.defineState(State("idle_right", 0, {Point(0, 0)}));
	animator.defineState(State("move_left", 0.5, Point(0, 1), 4, true));
	animator.defineState(State("move_right", 0.5, Point(0, 0), 4, true));
	animator.transition(Random::range(0.0, 1.0) <= 0.5 ? "idle_left" : "idle_right");

	return wolf;
}