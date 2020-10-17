#include "systems/sprite_system.h"

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
	vector<Actor*> actors;

	Actors::forEach<Sprite, Rect>([&](Actor &actor) {
		actors.push_back(&actor);
	});

	sort(actors.begin(), actors.end(), [&](Actor *lhs, Actor *rhs) {
		auto leftDepth = lhs->get<Sprite>().depth;
		auto rightDepth = rhs->get<Sprite>().depth;

		return leftDepth < rightDepth;
	});

	auto center = Display::center;
	auto screenSize = Display::getScreenSize();
	auto width = get<0>(screenSize);
	auto height = get<1>(screenSize);

	for(auto actor : actors) {
		auto &sprite = actor->get<Sprite>();
		auto &rect = actor->get<Rect>();

		SDL_Rect source;
		source.x = sprite.uv.x;
		source.y = sprite.uv.y;
		source.w = sprite.size.x;
		source.h = sprite.size.y;

		SDL_Rect dest;
		dest.x = width / 2 + rect.position.x - rect.size.x / 2 - center.x;
		dest.y = height / 2 - rect.position.y - rect.size.y / 2 + center.y;
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