#include "scenes/dungeon/skeleton.h"

#include <cmath>
#include <mutex>
#include <string>
#include "actor.h"
#include "engine.h"
#include "components/player.h"
#include "components/rect.h"
#include "components/controller.h"
#include "components/collider.h"
#include "components/focus.h"
#include "components/motion.h"
#include "components/sprite.h"
#include "components/animator.h"
#include "components/tags.h"
#include "structures/texture.h"
#include "structures/point.h"
#include "structures/color.h"
#include "system.h"
#include "utils/moth.h"
#include "utils/timer.h"

#include "components/mob.h"
#include "systems/mob_system.h"
#include "scenes/dungeon/health.h"
#include "scenes/dungeon/pathfinder.h"

using namespace std;
using namespace Moth;
using Systems = Engine::Systems;
using Actors = Engine::Actors;
using Input = Engine::Input;
using Display = Engine::Display;

once_flag skeletonFlag;

Actor& Skeleton::spawn(Vector2 position) {
	call_once(skeletonFlag, []() {
		MobSystem::setBehavior("skeleton", [](Actor &skeleton, int tick) {
			auto &skeRect = skeleton.get<Rect>();
			auto &controller = skeleton.get<Controller>();

			controller.move = Vector2::zero;

			Actor *closestPlayer = nullptr;
			float distance;

			Actors::forEach<Player>([&](Actor &actor) {
				if(closestPlayer == nullptr) {
					closestPlayer = &actor;
					distance = (skeRect.position - actor.get<Rect>().position).magnitude();
				} else {
					auto otherDistance = (skeRect.position - actor.get<Rect>().position).magnitude();

					if(otherDistance < distance) {
						closestPlayer = &actor;
						distance = otherDistance;
					}
				}
			});

			if(closestPlayer == nullptr)
				return;

			auto &target = *closestPlayer;

			if(distance > 500)
				return;

			skeleton.get<Pathfinder>().moveTo(target);
		});
	});

	//Create the player
	auto &skeleton = Actor::spawn("Skeleton");
	skeleton.add(new Rect(position, Vector2::one * 40));
	skeleton.add(new Controller(4));
	skeleton.add(new Mob("skeleton"));
	skeleton.add<Pathfinder>();

	//Health aspects
	auto &health = skeleton.add(new Health(8));
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
	auto &collider = skeleton.add(new Collider(Vector2(27.5, 40)));
	collider.respond = [](Actor &actor, Actor &hit) {
		Collider::defaultResponse(actor, hit);

		if(!hit.has<Player>())
			return;

		hit.get<Health>().damage(2);
	};

	//Add movement constraints
	auto &motion = skeleton.add<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 175;
	motion.friction = 0.025;

	//Describe sprite sheet
	auto &sprite = skeleton.add(new Sprite(Texture("./resources/mobs/skeleton.png"), 1));
	sprite.size = Vector2::one * 16;

	//Define animation state machine
	auto &animator = skeleton.add(new Animator([](Actor &actor, string currentState) {
		auto &controller = actor.get<Controller>();
		auto move = controller.move;
		auto lastMove = controller.lastMove;

		if(move.sqrMagnitude() > 0) {
			if(move.y != 0) {
				if(lastMove != move) {
					switch(sign(move.y)) {
						case -1:
							return "move";
						case 1:
							return "move_up";
					}
				}
			} else if(lastMove.x != move.x) {
				switch(sign(move.x)) {
					case -1:
					case 1:
						return "move";
				}
			}
		} else {
			switch(sign(lastMove.y)) {
				case -1:
					return "idle";
				case 1:
					return "idle_up";
			}

			switch(sign(lastMove.x)) {
				case -1:
				case 1:
					return "idle";
			}
		}

		return currentState.c_str();
	}));

	//Describe animation states
	auto count = 2;
	auto duration = 0.5;
	animator.defineState(State("idle_up", duration, {Point(0, 0)}));
	animator.defineState(State("idle", duration, {Point(0, 1)}));
	animator.defineState(State("move_up", duration, Point(1, 0), count, true));
	animator.defineState(State("move", duration, Point(1, 1), count, true));
	animator.transition("idle");

	return skeleton;
}