#pragma once

#include <vector>
#include "generation/noise.h"

/**
 * Creates value noise
 */
class ValueNoise : public Noise {
	public:
		ValueNoise(int width, int height, int seed);
		float sample(float x, float y) const override;
	private:
		std::vector<float> values;
};