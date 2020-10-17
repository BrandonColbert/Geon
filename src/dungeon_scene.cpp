#include "scenes/dungeon_scene.h"

#include "actor.h"
#include "animation/state.h"
#include "components/animator.h"
#include "components/controller.h"
#include "components/motion.h"
#include "components/player.h"
#include "components/rect.h"
#include "components/sprite.h"
#include "engine.h"
#include "graphics/texture.h"
#include "math/moth.h"
#include "math/vector2.h"

using Actors = Engine::Actors;
using Systems = Engine::Systems;

class SkeletonSystem : public System {
	void update() override {
		Actors::forEach<Player, Controller, Animator>([](Actor &actor) {
			auto &controller = actor.get<Controller>();
			auto &animator = actor.get<Animator>();

			auto move = controller.move;
			auto lastMove = controller.lastMove;

			if(move.sqr_magnitude() > 0) {
				if(move.x != 0) {
					if(lastMove.x != move.x) {
						switch(Moth::sign(move.x)) {
							case -1:
								animator.transition("left");
								break;
							case 1:
								animator.transition("right");
								break;
						}
					}
				} else if(lastMove != move) {
					switch(Moth::sign(move.y)) {
						case -1:
							animator.transition("down");
							break;
						case 1:
							animator.transition("up");
							break;
					}
				}
			} else {
				switch(Moth::sign(lastMove.y)) {
					case -1:
						animator.transition("idle_down");
						break;
					case 1:
						animator.transition("idle_up");
						break;
				}

				switch(Moth::sign(lastMove.x)) {
					case -1:
						animator.transition("idle_left");
						break;
					case 1:
						animator.transition("idle_right");
						break;
				}
			}
		});
	}
};

void DungeonScene::create() {
	Systems::add<SkeletonSystem>();

	auto &skeleton = Actor::spawn("Skeleton");
	skeleton.add(new Player());
	skeleton.add(new Sprite(Texture("./resources/skeleton.png"), 2));
	skeleton.add(new Rect(Vector2(100, 0), Vector2(150, 150)));
	skeleton.add(new Animator());
	skeleton.add(new Controller(8));
	skeleton.add(new Motion());

	auto &motion = skeleton.get<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 300;
	motion.friction = 0.975;

	auto &sprite = skeleton.get<Sprite>();
	sprite.size = Vector2(64, 64);

	auto count = 8;
	auto duration = 1;

	auto &animator = skeleton.get<Animator>();
	animator.defineState(State("idle_up", duration, {{0, 0}}));
	animator.defineState(State("idle_left", duration, {{0, 1}}));
	animator.defineState(State("idle_down", duration, {{0, 2}}));
	animator.defineState(State("idle_right", duration, {{0, 3}}));
	animator.defineState(State("up", duration, {1, 0}, count, true));
	animator.defineState(State("left", duration, {1, 1}, count, true));
	animator.defineState(State("down", duration, {1, 2}, count, true));
	animator.defineState(State("right", duration, {1, 3}, count, true));
	animator.transition("idle_down");
}