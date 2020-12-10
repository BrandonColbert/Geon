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
#include "generation/floor.h"
#include "structures/grid.h"
#include "structures/state.h"
#include "structures/texture.h"
#include "structures/vector2.h"
#include "structures/color.h"
#include "system.h"
#include "utils/format.h"
#include "utils/moth.h"
#include "engine.h"

#include <cmath>
#include "console.h"
#include "components/motion.h"
#include "components/emitter.h"
#include "utils/random.h"

using namespace std;
using namespace Format;
using namespace Moth;
using Actors = Engine::Actors;
using Systems = Engine::Systems;
using Input = Engine::Input;
using Display = Engine::Display;

class AttackSystem : public System {
	public:
		void update() override {
			Actors::forEach<Player>([](Actor &player) {
				//Detect spacebar press
				if(!Input::down("space"))
					return;

				auto &playerRect = player.get<Rect>();
				auto direction = (Display::screenToWorld(Input::mousePosition) - playerRect.position).normalized();

				//Make the fireball
				auto &fireball = Actor::spawn("Fireball");
				auto &rect = fireball.add(new Rect(
					playerRect.position + direction * playerRect.size.magnitude() / 4,
					Vector2::one * 20
				));

				//Set its velocity
				auto &motion = fireball.add<Motion>();
				motion.velocity = direction * 400;

				//Emit flame particles
				auto &emitter = fireball.add<Emitter>();
				emitter.depth = 2;
				emitter.frequency = 50;
				emitter.particleLifespan = []() { return Random::range(0.5f, 1.0f); };
				emitter.emitTexture = []() { return Texture(1, 1, Color::white); };
				emitter.emitPosition = [&]() { return fireball.get<Rect>().position; };

				emitter.emitVelocity = [&]() {
					auto direction = fireball.get<Motion>().velocity.normalized();
					auto angle = atan2(direction.y, direction.x) + Random::range(-M_PI / 6, M_PI / 6);

					return Vector2(cos(angle), sin(angle)).normalized() * -100;
				};

				emitter.decay = [](Actor &actor, float lifespan, float lifetime) {
					auto t = lifetime / lifespan;
					actor.get<Rect>().size = Vector2(20, 20) * lerp<float>(1, 0, pow(t, 2));
					actor.get<Sprite>().tint = Color::lerp(Color::red, Color(0xffc31e), pow(t, 0.3));

					return false;
				};

				//Extinguish on impact and destroy planks
				auto &collider = fireball.add(new Collider(rect.size));
				collider.respond = [&](Actor &actor, Actor &other) {
					if(other == player)
						return;

					actor.destroy();

					if(other.has<Sprite>() && other.get<Sprite>().texture.path == "./resources/tiles/plank.png") {
						auto &tile = Actor::spawn("Tile");
						tile.add(new Rect(other.get<Rect>().position, other.get<Rect>().size));
						tile.add(new Sprite(Texture("./resources/tiles/floor.png")));

						other.destroy();
					}
				};
			});
		}
};

void DungeonScene::create() {
	Display::zoom = 0.8;

	Systems::add<AttackSystem>();

	//Create the player
	auto &skeleton = Actor::spawn("Skeleton");
	skeleton.add(new Player());
	skeleton.add(new Rect(Vector2::zero, Vector2::one * 60));
	skeleton.add(new Controller(4));
	skeleton.add(new Collider(Vector2(25, 40)));
	skeleton.add(new Focus());

	//Add movement constraints
	auto &motion = skeleton.add<Motion>();
	motion.terminalVelocity = Vector2::one.normalized() * 175;
	motion.friction = 0.025;

	//Describe sprite sheet
	auto &sprite = skeleton.add(new Sprite(Texture("./resources/skeleton.png"), 1));
	sprite.size = Vector2::one * 64;
	sprite.offset = Vector2::up * 8;

	//Define animation state machine
	auto &animator = skeleton.add(new Animator([](Actor &actor, string currentState) {
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

	//Describe animation states
	auto count = 8;
	auto duration = 1;
	animator.defineState(State("idle_up", duration, {Point(0, 0)}));
	animator.defineState(State("idle_left", duration, {Point(0, 1)}));
	animator.defineState(State("idle_down", duration, {Point(0, 2)}));
	animator.defineState(State("idle_right", duration, {Point(0, 3)}));
	animator.defineState(State("up", duration, Point(1, 0), count, true));
	animator.defineState(State("left", duration, Point(1, 1), count, true));
	animator.defineState(State("down", duration, Point(1, 2), count, true));
	animator.defineState(State("right", duration, Point(1, 3), count, true));
	animator.transition("idle_down");

	//Create the map
	// Floor floor(100, 100, 1);
	// floor.rooms = 25;
	// floor.distribution = 10;
	// floor.spacing = 2;
	// floor.minRoomSideLength = 10;
	// floor.maxRoomSideLength = 20;

	// auto info = floor.generate();
	// auto tilemap = info.tilemap;
	// tilemap.tileSize = 50;
	// tilemap.actualize();

	Display::backgroundColor = Color(0.05, 0.05, 0.2);

	// auto mainRoom = info.rooms.front();

	// for(auto room : info.rooms)
	// 	if(room.region.size() > mainRoom.region.size())
	// 		mainRoom = room;

	// skeleton.get<Rect>().position = tilemap.tileToWorld(mainRoom.center);
	const auto n = Tilemap::Tile::None;
	const auto f = Tilemap::Tile::Floor;
	const auto w = Tilemap::Tile::Wall;

	Tilemap floor(Grid<Tilemap::Tile>{
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
	// floor.visualize();
}