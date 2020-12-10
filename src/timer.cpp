#include "utils/timer.h"

#include <cstdint>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_events.h>

using namespace std;

Timer::Timer(Callback callback) : callback(callback) {}

uint32_t callback(uint32_t interval, void *param) {
	SDL_UserEvent user;
	user.type = SDL_USEREVENT;
	user.code = 0;
	user.data1 = (void*)"timer";
	user.data2 = param;

	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user = user;

	SDL_PushEvent(&event);

	return 0;
}

void Timer::after(float time, Callback callback) {
	auto timer = new Timer(callback);
	SDL_AddTimer(time * 1000, ::callback, timer);
}