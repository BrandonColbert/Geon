#include "systems/overlay_system.h"

#include <SDL2/SDL_ttf.h>
#include "engine.h"

using namespace std;
using Display = Engine::Display;

std::vector<Overlay*> OverlaySystem::overlays;

class TextOverlay : public Overlay {
	public:
		Vector2 positon;

		TextOverlay(std::string text, Color color) {
			auto surface = TTF_RenderText_Solid(
				const_cast<TTF_Font*>(font),
				text.c_str(),
				color
			);

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
			rect.x = dim.x - width * 0.5;
			rect.y = dim.y - height * 0.5;
			rect.w = width;
			rect.h = height;

			SDL_RenderCopy(Engine::getInstance(), this->texture, NULL, &rect);
		}
	private:
		const TTF_Font *font = TTF_OpenFont("Sans.ttf", 24);
		SDL_Texture *texture;
};

OverlaySystem::OverlaySystem() {
	TTF_Init();
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
	overlay->positon = position;
	overlays.push_back(overlay);
}

Overlay::~Overlay() {}