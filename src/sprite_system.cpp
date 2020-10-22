#include "systems/sprite_system.h"

#include <algorithm>
#include <cmath>
#include <SDL2/SDL_image.h>
#include <vector>
#include "components/rect.h"
#include "components/sprite.h"
#include "engine.h"

#include "console.h"

using namespace std;
using Actors = Engine::Actors;
using Display = Engine::Display;

void SpriteSystem::render() {
	auto center = Display::center;
	auto screenSize = Display::getScreenSize();
	auto halfWidth = get<0>(screenSize) / 2;
	auto halfHeight = get<1>(screenSize) / 2;
	auto startX = halfWidth - center.x;
	auto startY = halfHeight + center.y;

	vector<Actor*> actors;

	Actors::forEach<Sprite, Rect>([&](Actor &actor) {
		// auto &rect = actor.get<Rect>();

		// if(rect.position.x < (center.x - halfWidth) || (center.x + halfWidth) < rect.position.x)
		// 	return;

		// if(rect.position.y < (center.y - halfHeight) || (center.y + halfHeight) < rect.position.y)
		// 	return;

		actors.push_back(&actor);
	});

	sort(actors.begin(), actors.end(), [&](Actor *lhs, Actor *rhs) {
		auto leftDepth = lhs->get<Sprite>().depth;
		auto rightDepth = rhs->get<Sprite>().depth;

		return leftDepth < rightDepth;
	});

	for(auto actor : actors) {
		auto &rect = actor->get<Rect>();
		auto &sprite = actor->get<Sprite>();

		SDL_Rect source;
		source.x = sprite.uv.x;
		source.y = sprite.uv.y;
		source.w = sprite.size.x;
		source.h = sprite.size.y;

		SDL_Rect dest;
		dest.x = startX + rect.position.x - rect.size.x / 2 + sprite.offset.x;
		dest.y = startY - rect.position.y - rect.size.y / 2 - sprite.offset.y;
		dest.w = rect.size.x;
		dest.h = rect.size.y;

		SDL_Point pivot;
		pivot.x = round(sprite.pivot.x);
		pivot.y = round(sprite.pivot.y);

		auto flipFlag = (int)SDL_FLIP_NONE;

		if(sprite.flipX)
			flipFlag |= (int)SDL_FLIP_HORIZONTAL;
		if(sprite.flipY)
			flipFlag |= (int)SDL_FLIP_VERTICAL;

		SDL_RenderCopyEx(
			Engine::getInstance(),
			sprite.texture,
			&source, &dest,
			rect.rotation,
			&pivot,
			(SDL_RendererFlip)flipFlag
		);
	}
}