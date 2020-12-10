#include "scenes/test_particles_scene.h"

#include <cmath>
#include "actor.h"
#include "components/emitter.h"
#include "components/rect.h"
#include "components/sprite.h"
#include "generation/tilemap.h"
#include "structures/grid.h"
#include "structures/color.h"
#include "utils/moth.h"
#include "utils/random.h"

using namespace std;
using namespace Moth;
using Tile = Tilemap::Tile;

void TestParticlesScene::create() {
	const auto n = Tile::None;
	const auto f = Tile::Floor;
	const auto w = Tile::Wall;

	Tilemap floor(Grid<Tile>{
		{n, n, n, n, n, n, n, n, n, n, n, n, n},
		{n, w, w, w, w, w, w, w, w, w, w, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, w, w, w, w, w, w, w, w, w, w, n},
		{n, n, n, n, n, n, n, n, n, n, n, n, n}
	});

	floor.tileSize = 50;
	floor.actualize();

	auto &particleEmitter = Actor::spawn("Emitter");
	particleEmitter.add(new Emitter());

	auto &emitter = particleEmitter.get<Emitter>();
	emitter.depth = 1;
	emitter.frequency = 30;
	emitter.particleLifespan = []() { return 1; };
	emitter.emitVelocity = []() { return Vector2(Random::range(-0.5, 0.5), 2) * 100; };
	emitter.emitTexture = []() { return Texture(1, 1, Color::white); };
	emitter.decay = [](Actor &actor, float lifespan, float lifetime) {
		auto t = lifetime / lifespan;
		actor.get<Rect>().size = Vector2(20, 20) * lerp<float>(1, 0, pow(t, 2));
		actor.get<Sprite>().tint = Color::lerp(Color::red, Color(0xffc31e), pow(t, 0.3));

		return false;
	};
}