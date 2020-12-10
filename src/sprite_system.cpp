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

float SpriteSystem::EdgeOverflow = 1.25;

void SpriteSystem::render() {
	auto zoom = Display::zoom;
	auto center = Display::center;
	auto res = Display::targetResolution;
	auto screenSize = Display::getScreenSize();

	auto scale = (float)screenSize.x / (float)res.x * zoom;
	auto ratio = (float)screenSize.x / (float)screenSize.y;

	auto halfWidth = screenSize.x / scale / 2;
	auto halfHeight = screenSize.x / scale / ratio / 2;
	auto startX = halfWidth - center.x;
	auto startY = halfHeight + center.y;

	vector<Actor*> actors;

	Actors::forEach<Sprite, Rect>([&](Actor &actor) {
		auto &rect = actor.get<Rect>();

		if(rect.position.x < (center.x - halfWidth * EdgeOverflow) || (center.x + halfWidth * EdgeOverflow) < rect.position.x)
			return;

		if(rect.position.y < (center.y - halfHeight * EdgeOverflow) || (center.y + halfHeight * EdgeOverflow) < rect.position.y)
			return;

		actors.push_back(&actor);
	});

	sort(actors.begin(), actors.end(), [&](Actor *lhs, Actor *rhs) {
		return lhs->get<Sprite>().depth < rhs->get<Sprite>().depth;
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
		dest.x = (startX + rect.position.x - rect.size.x / 2 + sprite.offset.x) * scale;
		dest.y = (startY - rect.position.y - rect.size.y / 2 - sprite.offset.y) * scale;
		dest.w = rect.size.x * scale;
		dest.h = rect.size.y * scale;

		SDL_Point pivot;
		pivot.x = round(sprite.pivot.x * scale);
		pivot.y = round(sprite.pivot.y * scale);

		auto flipFlag = (int)SDL_FLIP_NONE;

		if(sprite.flipX)
			flipFlag |= (int)SDL_FLIP_HORIZONTAL;
		if(sprite.flipY)
			flipFlag |= (int)SDL_FLIP_VERTICAL;

		SDL_Color tint = sprite.tint;
		SDL_SetTextureColorMod(sprite.texture, tint.r, tint.g, tint.b);
		SDL_SetTextureAlphaMod(sprite.texture, tint.a);

		SDL_RenderCopyEx(
			Engine::getInstance(),
			sprite.texture,
			&source, &dest,
			rect.rotation,
			&pivot,
			(SDL_RendererFlip)flipFlag
		);

		// SDL_SetTextureColorMod(sprite.texture, white.r, white.g, white.b);
		// SDL_SetTextureAlphaMod(sprite.texture, white.a);
	}

	// static int frame = 0;
	// if(frame++ % 20 == 0)
	// 	Console::print("Sprites: %, FPS: %", actors.size(), Engine::Time::fps);
}