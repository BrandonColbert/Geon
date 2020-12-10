#include "scenes/dungeon/warrior.h"

#include <cmath>
#include <mutex>
#include <string>
#include <sstream>
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
#include <utils/format.h>
#include "systems/overlay_system.h"

#include "scenes/dungeon/health.h"
#include "scenes/dungeon_scene.h"

using namespace std;
using namespace Moth;
using Systems = Engine::Systems;
using Actors = Engine::Actors;
using Input = Engine::Input;
using Display = Engine::Display;
using Time = Engine::Time;

once_flag warriorFlag;

float nextAttackTime = 0;

struct WarriorSystem : public System {
	void update() override {
		Actors::forEach<Player>([](Actor &player) {
			//Attack
			if(Input::down("space") && Time::time > nextAttackTime) {
				nextAttackTime = Time::time + 0.5;

				auto &playerRect = player.get<Rect>();
				auto direction = (Display::screenToWorld(Input::mousePosition) - playerRect.position).normalized();

				auto &arrow = Actor::spawn("Arrow");

				arrow.add(new Tags{"player_projectile"});

				auto &rect = arrow.add(new Rect(Vector2::zero, Vector2::one * 20));
				rect.position = playerRect.position + direction * playerRect.size.magnitude() / 2;
				rect.rotation = 45 - atan2(direction.y, direction.x) * 180 / M_PI;

				auto &motion = arrow.add<Motion>();
				motion.velocity = direction * 800;

				auto &sprite = arrow.add(new Sprite(Texture("./resources/mobs/arrow.png"), 1));
				sprite.pivot = rect.size / 2;

				auto &collider = arrow.add(new Collider(rect.size));
				collider.respond = [&](Actor &actor, Actor &hit) {
					if(hit == player)
						return;

					if(hit.has<Health>())
						hit.get<Health>().damage(1);

					if(hit.has<Motion>())
						hit.get<Motion>().velocity += actor.get<Motion>().velocity;

					actor.destroy();
				};
			}

			//Health
			auto &health = player.get<Health>();
			stringstream hp;
			Format::formatTo(hp, "HP: %/%", health.current(), health.max());
			
			OverlaySystem::text(hp.str(), Vector2(-0.75, -0.75), Color::white);

			//Remaining
			stringstream remaining;
			Format::formatTo(remaining, "Enemies Left: %", DungeonScene::remainingMobs);
			
			OverlaySystem::text(remaining.str(), Vector2(0, 0.8), Color::white);
		});
	}
};

Actor& Warrior::spawn(Vector2 position) {
	call_once(warriorFlag, []() {
		Systems::add<WarriorSystem>();
	});

	//Create the player
	auto &player = Actor::spawn("Player");
	player.add(new Player());
	player.add(new Rect(position, Vector2::one * 40));
	player.add(new Controller(4));
	player.add(new Focus());

	//Collisions to account for self-shot projectiles
	auto &collider = player.add(new Collider(Vector2(27.5, 40)));
	collider.respond = [](Actor &actor, Actor &hit) {
		if(hit.has<Tags>() && hit.get<Tags>().has("player_projectile"))
			return;

		Collider::defaultResponse(actor, hit);
	};

	//Health aspects
	auto &health = player.add(new Health(10));
	health.invincibilityDuration = 0.8;
	health.onChange = [](Actor &actor, float oldValue, float newValue) {
		if(newValue >= oldValue)
			return;

		Console::print("% HP: % -> %", actor.name, oldValue, newValue);

		actor.get<Sprite>().tint = Color(1, 0.5, 0.5);

		Timer::after(actor.get<Health>().invincibilityDuration * 0.5, [&]() {
			if(!actor.exists())
				return;

			actor.get<Sprite>().tint = Color::white;
		});
	};

	//Add movement constraints
	auto &motion = player.add<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 175;
	motion.friction = 0.025;

	//Describe sprite sheet
	auto &sprite = player.add(new Sprite(Texture("./resources/mobs/warrior.png"), 1));
	sprite.size = Vector2::one * 16;

	//Define animation state machine
	auto &animator = player.add(new Animator([](Actor &actor, string currentState) {
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

	return player;
}