#include "systems/focus_system.h"

#include "components/focus.h"
#include "components/rect.h"
#include "engine.h"
#include "structures/vector2.h"

using Actors = Engine::Actors;
using Display = Engine::Display;

void FocusSystem::update() {
	Actors::forEach<Focus, Rect>([](Actor &actor) {
		auto &rect = actor.get<Rect>();
		Display::center = rect.position;
	});
}