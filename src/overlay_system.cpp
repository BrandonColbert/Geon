#include "systems/overlay_system.h"

#include <SDL2/SDL_ttf.h>
#include "engine.h"

using namespace std;
using Display = Engine::Display;

std::vector<Overlay*> OverlaySystem::overlays;
TTF_Font *font;

class TextOverlay : public Overlay {
	public:
		Vector2 position;

		TextOverlay(std::string text, Color color) {
			auto surface = TTF_RenderText_Solid(font, text.c_str(), color);

			this->texture = SDL_CreateTextureFromSurface(Engine::getInstance(), surface);

			SDL_FreeSurface(surface);
		}

		~TextOverlay() override {
			SDL_DestroyTexture(this->texture);
		}

		void render() override {
			auto dim = Display::getScreenSize();

			int width, height;
			SDL_QueryTexture(texture, NULL, NULL, &width, &height);

			SDL_Rect rect;
			rect.x = dim.x / 2 - width / 2 + dim.x / 2 * position.x;
			rect.y = dim.y / 2 - height / 2 - dim.y / 2 * position.y;
			rect.w = width;
			rect.h = height;

			SDL_RenderCopy(Engine::getInstance(), this->texture, NULL, &rect);
		}
	private:
		SDL_Texture *texture;
};

OverlaySystem::OverlaySystem() {
	TTF_Init();
	font  = TTF_OpenFont("./resources/sans.ttf", 24);
}

OverlaySystem::~OverlaySystem() {
	TTF_Quit();
}

void OverlaySystem::render() {
	for(auto overlay : overlays)
		overlay->render();

	for(auto overlay : overlays)
		delete overlay;

	overlays.clear();
}

void OverlaySystem::text(string text, Vector2 position, Color color) {
	auto overlay = new TextOverlay(text, color);
	overlay->position = position;
	overlays.push_back(overlay);
}

Overlay::~Overlay() {}