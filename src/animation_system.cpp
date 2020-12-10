#include "systems/animation_system.h"

#include "components/animator.h"
#include "components/sprite.h"
#include "engine.h"
#include "structures/vector2.h"

using namespace std;
using Actors = Engine::Actors;

void AnimationSystem::update() {
	Actors::forEach<Animator, Sprite>([](Actor &actor) {
		auto &animator = actor.get<Animator>();
		auto &sprite = actor.get<Sprite>();

		auto newState = animator.evaluateState();
		if(newState != animator.getActiveStateName())
			animator.transition(newState);

		if(!animator.hasState())
			return;

		auto &state = animator.getActiveState();
		auto coords = state.getCoords(animator.getStep());
		sprite.uv = Vector2(sprite.size.x * coords.x, sprite.size.y * coords.y);
	});
}