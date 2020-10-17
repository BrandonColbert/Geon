#include "engine.h"

using namespace std;
using Display = Engine::Display;

Vector2 Display::center;
float Display::zoom = 1;

tuple<int, int> Display::getScreenSize() {
	int width, height;
	SDL_GetRendererOutputSize(getInstance().renderer, &width, &height);

	return make_tuple(width, height);
}