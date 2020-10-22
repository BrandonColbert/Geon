#include "scenes/dungeon_scene.h"

#include "actor.h"
#include "components/animator.h"
#include "components/collider.h"
#include "components/controller.h"
#include "components/focus.h"
#include "components/motion.h"
#include "components/player.h"
#include "components/rect.h"
#include "components/sprite.h"
#include "generation/tilemap.h"
#include "structures/grid.h"
#include "structures/state.h"
#include "structures/texture.h"
#include "structures/vector2.h"
#include "utils/format.h"
#include "utils/moth.h"

using namespace std;
using namespace Format;
using Tile = Tilemap::Tile;

void DungeonScene::create() {
	auto &skeleton = Actor::spawn("Skeleton");
	skeleton.add(new Player());
	skeleton.add(new Sprite(Texture("./resources/skeleton.png"), 1));
	skeleton.add(new Rect(Vector2::zero, Vector2::one * 75));
	skeleton.add(new Controller(4));
	skeleton.add(new Motion());
	skeleton.add(new Collider(Vector2(25, 50)));
	skeleton.add(new Focus());

	auto &motion = skeleton.get<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 150;
	motion.friction = 0.975;

	auto &sprite = skeleton.get<Sprite>();
	sprite.size = Vector2::one * 64;
	sprite.offset = Vector2::up * 8;

	auto count = 8;
	auto duration = 1;

	skeleton.add(new Animator([](Actor &actor, string currentState) {
		auto &controller = actor.get<Controller>();
		auto move = controller.move;
		auto lastMove = controller.lastMove;

		if(move.sqrMagnitude() > 0) {
			if(move.x != 0) {
				if(lastMove.x != move.x) {
					switch(Moth::sign(move.x)) {
						case -1:
							return "left";
						case 1:
							return "right";
					}
				}
			} else if(lastMove != move) {
				switch(Moth::sign(move.y)) {
					case -1:
						return "down";
					case 1:
						return "up";
				}
			}
		} else {
			switch(Moth::sign(lastMove.y)) {
				case -1:
					return "idle_down";
				case 1:
					return "idle_up";
			}

			switch(Moth::sign(lastMove.x)) {
				case -1:
					return "idle_left";
				case 1:
					return "idle_right";
			}
		}

		return currentState.c_str();
	}));

	auto &animator = skeleton.get<Animator>();
	animator.defineState(State("idle_up", duration, {make_tuple(0, 0)}));
	animator.defineState(State("idle_left", duration, {make_tuple(0, 1)}));
	animator.defineState(State("idle_down", duration, {make_tuple(0, 2)}));
	animator.defineState(State("idle_right", duration, {make_tuple(0, 3)}));
	animator.defineState(State("up", duration, make_tuple(1, 0), count, true));
	animator.defineState(State("left", duration, make_tuple(1, 1), count, true));
	animator.defineState(State("down", duration, make_tuple(1, 2), count, true));
	animator.defineState(State("right", duration, make_tuple(1, 3), count, true));
	animator.transition("idle_down");

	const auto n = Tile::None;
	const auto f = Tile::Floor;
	const auto w = Tile::Wall;

	Tilemap floor(Grid<Tile>{
		{n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n},
		{n, n, n, n, n, n, n, n, n, n, n, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, n},
		{n, n, n, n, n, n, n, n, n, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, w, w, w, w, w, w, w, w, w, n},
		{n, w, w, w, w, w, w, w, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, w, w, w, w, w, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, w, w, w, f, f, w, w, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, n, n, w, f, f, w, n, n, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, n, n, w, f, f, w, n, n, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, w, f, f, w, n},
		{n, w, w, w, f, f, w, w, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, w, f, f, w, w, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, w, n, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, w, n, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, n, n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, w, n, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, w, w, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, w, n, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, w, w, w, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, w, w, w, w, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, f, w, f, f, f, f, f, f, f, w, n},
		{n, w, w, w, w, w, w, w, w, n, n, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, w, n},
		{n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n}
	});

	floor.tileSize = 50;
	floor.actualize();
	floor.visualize();
}