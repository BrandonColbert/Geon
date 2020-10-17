#include "animation/state.h"

using namespace std;

State::State(string name, float duration, vector<tuple<int, int>> steps, bool loops) {
	this->name = name;
	this->duration = duration;
	this->loops = loops;
	this->steps = steps;
}

State::State(string name, float duration, tuple<int, int> start, int count, bool loops) {
	this->name = name;
	this->duration = duration;
	this->loops = loops;

	auto [x, y] = start;
	for(auto i = 0; i < count; i++)
		steps.push_back({x + i, y});
}

State::State(string name, float duration, int y, int count, bool loops) : State(name, duration, {0, y}, count, loops) {}

State::State() : State("", 0) {}

string State::getName() const {
	return name;
}

tuple<int, int> State::getCoords(int step) const {
	return steps[step];
}

int State::getStepCount() const {
	return steps.size();
}