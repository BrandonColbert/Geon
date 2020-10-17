#include "engine.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <sstream>
#include <vector>
#include "components/sprite.h"
#include "console.h"
#include "utils/printer.h"

using namespace std;
using Time = Engine::Time;

void Engine::setupDisplay(string name, int width, int height) {
	if(isDisplayed)
		throw runtime_error("Display has already been setup");

	isDisplayed = true;

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Console::error("Error initializing SDL: %", SDL_GetError());
		return;
	}

	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow(
		name.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		0
	);

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);
}

Engine::Engine() {
	srand(time(nullptr));
}

Engine::~Engine() {
	if(isDisplayed) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		IMG_Quit();
		SDL_Quit();
	}

	for(auto pair : actors)
		delete pair.second;

	for(auto system : systems)
		delete system;
}

void Engine::registerActor(Actor *actor) {
	if(actors.find(actor->id) != actors.end()) {
		stringstream stream;
		Printer::printTo(stream, "Attempted to manually register Actor % to engine\n", actor->id);
		throw runtime_error(stream.str().c_str());
	}

	actors.insert({actor->id, actor});
}

void Engine::handleEvents() {
	downKeys.clear();
	upKeys.clear();

	while(SDL_PollEvent(&currentEvent)) {
		switch(currentEvent.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN: {
				auto keyCode = currentEvent.key.keysym.sym;
				downKeys.insert(keyCode);
				heldKeys.insert(keyCode);
				break;
			}
			case SDL_KEYUP: {
				auto keyCode = currentEvent.key.keysym.sym;
				upKeys.insert(keyCode);
				heldKeys.erase(keyCode);
				break;
			}
		}
	}
}

void Engine::update() {
	for(auto system : systems)
		system->update();

	auto next = actors.cbegin();

	while(next != actors.cend()) {
		auto id = next->first;
		auto actor = next->second;
		next++;

		actor->clean();

		if(!actor->exists()) {
			actors.erase(id);
			delete actor;
		}
	}
}

void Engine::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for(auto system : systems)
		system->render();

	SDL_RenderPresent(renderer);
}

void Engine::clock() {
	auto now = SDL_GetTicks() / 1000.0;
	deltaTime = now - epoch;
	epoch = now;

	if(Time::targetFps == 0)
		return;

	auto targetDeltaTime = 1.0 / Time::targetFps;
	if(deltaTime < targetDeltaTime)
		SDL_Delay(targetDeltaTime - deltaTime);
}

Engine::operator SDL_Window*() const {
	return window;
}

Engine::operator SDL_Renderer*() const {
	return renderer;
}

Engine& Engine::getInstance() {
	static Engine instance;

	return instance;
}