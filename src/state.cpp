#include "structures/state.h"

using namespace std;

State::State(string name, float duration, vector<Point> steps, bool loops) {
	this->name = name;
	this->duration = duration;
	this->loops = loops;
	this->steps = steps;
}

State::State(string name, float duration, Point start, int count, bool loops) {
	this->name = name;
	this->duration = duration;
	this->loops = loops;

	for(auto i = 0; i < count; i++)
		steps.push_back(Point(start.x + i, start.y));
}

State::State(string name, float duration, int y, int count, bool loops) : State(name, duration, Point(0, y), count, loops) {}

State::State() : State("", 0) {}

string State::getName() const {
	return name;
}

Point State::getCoords(int step) const {
	return steps[step];
}

int State::getStepCount() const {
	return steps.size();
}