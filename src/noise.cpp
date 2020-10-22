#include "generation/noise.h"

#include <tuple>

using namespace std;

Noise::Noise(int width, int height, int seed) : rand(seed) {
	this->width = width;
	this->height = height;
}

float Noise::sample(tuple<float, float> point) const {
	return sample(get<0>(point), get<1>(point));
}

Grid<float> Noise::toGrid() {
	Grid<float> grid(width, height);

	grid.populate([&](auto x, auto y) -> float {
		return sample((float)x / (float)width, (float)y / (float)height);
	});

	return grid;
}

int Noise::getWidth() const {
	return width;
}

int Noise::getHeight() const {
	return height;
}

float Noise::operator[](Vector2 v) {
	return sample(v.x, v.y);
}