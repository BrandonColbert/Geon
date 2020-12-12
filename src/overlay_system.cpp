#include "systems/overlay_system.h"

#include <SDL2/SDL_ttf.h>
#include "engine.h"

using namespace std;
using Display = Engine::Display;

std::vector<Overlay*> OverlaySystem::overlays;
TTF_Font *font;

class TextureOverlay : public Overlay {
	public:
		Vector2 position, scale;
		TextureOverlay(Texture texture) : texture(texture) {}

		void render() override {
			auto dim = Display::getScreenSize();
			auto res = Display::targetResolution;

			auto ratio = dim.x / res.x;

			auto width = texture.getWidth() * scale.x * ratio;
			auto height = texture.getHeight() * scale.y * ratio;

			SDL_Rect rect;
			rect.x = dim.x / 2 - width / 2 + dim.x / 2 * position.x;
			rect.y = dim.y / 2 - height / 2 - dim.y / 2 * position.y;
			rect.w = width;
			rect.h = height;

			SDL_RenderCopy(Engine::getInstance(), texture, NULL, &rect);
		}
	protected:
		Texture texture;
};

class TextOverlay : public Overlay {
	public:
		Vector2 position;
		float scale;

		TextOverlay(std::string text, Color color) {
			auto surface = TTF_RenderText_Solid(font, text.c_str(), color);
			texture = SDL_CreateTextureFromSurface(Engine::getInstance(), surface);

			SDL_FreeSurface(surface);
		}

		~TextOverlay() override {
			SDL_DestroyTexture(texture);
		}

		void render() override {
			auto dim = Display::getScreenSize();
			auto res = Display::targetResolution;

			int width, height;
			SDL_QueryTexture(texture, NULL, NULL, &width, &height);

			auto mod = (dim.x / res.x) * 0.25 * scale;
			width *= mod;
			height *= mod;

			SDL_Rect rect;
			rect.x = dim.x / 2 - width / 2 + dim.x / 2 * position.x;
			rect.y = dim.y / 2 - height / 2 - dim.y / 2 * position.y;
			rect.w = width;
			rect.h = height;

			SDL_RenderCopy(Engine::getInstance(), texture, NULL, &rect);
		}
	protected:
		SDL_Texture *texture;
};

class BoxOverlay : public Overlay {
	public:
		Vector2 position, size;
		Color color;

		BoxOverlay(Color color) : color(color) {}

		void render() override {
			auto dim = Display::getScreenSize();

			SDL_Rect rect;
			rect.x = dim.x / 2 - size.x / 2 + dim.x / 2 * position.x;
			rect.y = dim.y / 2 - size.y / 2 - dim.y / 2 * position.y;
			rect.w = size.x;
			rect.h = size.y;

			auto argb = color.getARGB();
			auto a = (argb >> 24) & 0xff;
			auto r = (argb >> 16) & 0xff;
			auto g = (argb >> 8) & 0xff;
			auto b = argb & 0xff;

			SDL_SetRenderDrawColor(Engine::getInstance(), r, g, b, a);
			SDL_RenderFillRect(Engine::getInstance(), &rect);
		}
};

Overlay::~Overlay() {}

OverlaySystem::OverlaySystem() {
	TTF_Init();
	font = TTF_OpenFont("./resources/sans.ttf", 100);
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

void OverlaySystem::text(string text, Vector2 position, Color color, float scale) {
	auto overlay = new TextOverlay(text, color);
	overlay->position = position;
	overlay->scale = scale;
	overlays.push_back(overlay);
}

void OverlaySystem::box(Vector2 position, Vector2 size, Color color) {
	auto overlay = new BoxOverlay(color);
	overlay->position = position;
	overlay->size = size;
	overlays.push_back(overlay);
}

void OverlaySystem::texture(Texture texture, Vector2 position, Vector2 scale) {
	auto overlay = new TextureOverlay(texture);
	overlay->position = position;
	overlay->scale = scale;
	overlays.push_back(overlay);
}