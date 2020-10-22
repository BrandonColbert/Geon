#pragma once

#include "structures/grid.h"
#include "structures/vector2.h"
#include "utils/random.h"

/**
 * Allows random generation of values according to a noise function
 */
class Noise {
	public:
		/**
		 * @param seed Seed to generate noise with
		 */
		Noise(int width, int height, int seed);

		/**
		 * @param x X coordinate
		 * @param y Y coordinate
		 * @return Noise value at point
		 */
		virtual float sample(float x, float y) const = 0;

		/**
		 * @param point Coordinate to get the value at
		 * @return Noise value at point
		 */
		float sample(std::tuple<float, float> point) const;

		int getWidth() const;

		int getHeight() const;

		/**
		 * @return A new grid with the noise values
		 */
		Grid<float> toGrid();

		float operator[](Vector2);
	protected:
		Random::Sequence rand;
	private:
		int width, height;
};