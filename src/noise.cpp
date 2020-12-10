#include "generation/noise.h"

using namespace std;

Noise::Noise(int width, int height, int seed) : rand(seed) {
	this->width = width;
	this->height = height;
}

float Noise::sample(Vector2 point) const {
	return sample(point.x, point.y);
}

Grid<float> Noise::toGrid() {
	Grid<float> grid(width, height);

	grid.populate([&](int x, int y) -> float {
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