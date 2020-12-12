#include "engine.h"

using Time = Engine::Time;

int Time::targetFps = 0;

Property<float> Time::deltaTime([]() {
	return Engine::getInstance().deltaTime;
});

Property<int> Time::fps([]() {
	return 1.0f / Engine::getInstance().deltaTime;
});

Property<float> Time::time([]() {
	return Engine::getInstance().time;
});