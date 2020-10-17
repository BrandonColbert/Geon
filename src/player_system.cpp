#include "systems/player_system.h"

#include "components/controller.h"
#include "components/player.h"
#include "engine.h"
#include "math/vector2.h"

using namespace std;
using Actors = Engine::Actors;
using Input = Engine::Input;
using Time = Engine::Time;

void PlayerSystem::update() {
	auto move = Vector2(
		(Input::held("a") ? -1 : 0) + (Input::held("d") ? 1 : 0),
		(Input::held("w") ? 1 : 0) + (Input::held("s") ? -1 : 0)
	);

	Actors::forEach<Player, Controller>([&](Actor &actor) {
		auto &controller = actor.get<Controller>();

		if(move.sqr_magnitude() > 0)
			move = move.normalized();

		controller.move = move;
	});
}