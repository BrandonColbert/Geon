#include "generation/value_noise.h"

#include <algorithm>
#include <cmath>
#include <vector>
#include "utils/moth.h"
#include "utils/random.h"

#include "console.h"

using namespace std;
using Moth::lerp;
using Random::Sequence;

ValueNoise::ValueNoise(int width, int height, int seed) : Noise(width, height, seed) {
	values = rand.take(width * height, 0.0f, 1.0f);
}

inline float smoothstep(const float &t) {
	return t * t * (3 - 2 * t);
} 

float ValueNoise::sample(float x, float y) const {
	x *= getWidth();
	y *= getHeight();

	auto xi = (int)std::floor(x);
	auto yi = (int)std::floor(y);

	auto tx = x - xi;
	auto ty = y - yi;

	auto rx0 = xi & getWidth();
	auto rx1 = (rx0 + 1) & getWidth();
	auto ry0 = yi & getHeight();
	auto ry1 = (ry0 + 1) & getHeight();

	auto c00 = values[ry0 * getWidth() + rx0];
	auto c10 = values[ry0 * getWidth() + rx1];
	auto c01 = values[ry1 * getWidth() + rx0];
	auto c11 = values[ry1 * getWidth() + rx1];

	auto sx = smoothstep(tx);
	auto sy = smoothstep(ty);

	auto nx0 = lerp(c00, c10, sx);
	auto nx1 = lerp(c01, c11, sx);

	return lerp(nx0, nx1, sy);
}