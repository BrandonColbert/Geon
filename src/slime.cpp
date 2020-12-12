#include "scenes/dungeon/slime.h"

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

#include "components/mob.h"
#include "systems/mob_system.h"
#include "scenes/dungeon/health.h"
#include "scenes/dungeon/pathfinder.h"
#include "scenes/dungeon_scene.h"

using namespace std;
using Actors = Engine::Actors;

once_flag slimeFlag;

Actor& Slime::spawn(Vector2 position) {
	call_once(slimeFlag, []() {
		MobSystem::setBehavior("slime", [](Actor &slime, int tick) {
			auto &controller = slime.get<Controller>();
			controller.move = Vector2::zero;

			Actor *closestPlayer = nullptr;
			float distance;

			Actors::forEach<Player>([&](Actor &actor) {
				if(closestPlayer == nullptr) {
					closestPlayer = &actor;
					distance = (slime.get<Rect>().position - actor.get<Rect>().position).magnitude();
				} else {
					auto otherDistance = (slime.get<Rect>().position - actor.get<Rect>().position).magnitude();

					if(otherDistance < distance) {
						closestPlayer = &actor;
						distance = otherDistance;
					}
				}
			});

			if(closestPlayer == nullptr)
				return;

			auto &target = *closestPlayer;
			slime.get<Pathfinder>().chase(target);
		});
	});

	//Create the player
	auto &slime = Actor::spawn("Slime");
	slime.add(new Rect(position, Vector2::one * 20));
	slime.add(new Controller(4));
	slime.add(new Mob("slime"));
	slime.add(new Pathfinder(DungeonScene::tilemap, 300));

	//Health aspects
	auto &health = slime.add(new Health(3));
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
	auto &collider = slime.add(new Collider(Vector2(20, 13.75)));
	collider.respond = [](Actor &actor, Actor &hit) {
		Collider::defaultResponse(actor, hit);

		if(!hit.has<Player>())
			return;

		hit.get<Health>().damage(1);
	};

	//Add movement constraints
	auto &motion = slime.add<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 175;
	motion.friction = 0.05;

	//Describe sprite sheet
	auto &sprite = slime.add(new Sprite(Texture("./resources/mobs/slime.png"), 1));
	sprite.size = Vector2::one * 16;
	sprite.offset = Vector2::up * 6.25;

	//Define animation state machine
	auto &animator = slime.add(new Animator([](Actor &actor, string currentState) {
		return actor.get<Controller>().move.sqrMagnitude() > 0 ? "move" : "idle";
	}));

	//Describe animation states
	animator.defineState(State("idle", 0, {Point(0, 0)}));
	animator.defineState(State("move", 0.75, Point(0, 0), 4, true));
	animator.transition("idle");

	return slime;
}