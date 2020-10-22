#include "structures/state.h"

#include <tuple>

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

	auto x = get<0>(start);
	auto y = get<1>(start);

	for(auto i = 0; i < count; i++)
		steps.push_back(make_tuple(x + i, y));
}

State::State(string name, float duration, int y, int count, bool loops) : State(name, duration, make_tuple(0, y), count, loops) {}

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