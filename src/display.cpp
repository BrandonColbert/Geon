#include "engine.h"

using namespace std;
using Display = Engine::Display;

Vector2 Display::center;
float Display::zoom = 1;
Point Display::targetResolution(640, 480);
Color Display::backgroundColor = Color::black;

Point Display::getScreenSize() {
	int width, height;
	SDL_GetRendererOutputSize(getInstance().renderer, &width, &height);

	return Point(width, height);
}

Vector2 Display::screenToWorld(Vector2 screenPoint) {
	return center + screenPoint;
}