#include "components/animator.h"

#include <algorithm>
#include <cmath>
#include "engine.h"

using namespace std;
using Time = Engine::Time;

Animator::Animator(StateMachine stateMachine) : stateMachine(stateMachine) {}

Animator::Animator() : stateMachine([](Actor &actor, string currentState) {
	return currentState;
}) {}

void Animator::transition(string state) {
	activeStateName = state;
	epoch = Time::time;
}

void Animator::defineState(State state) {
	states[state.getName()] = state;
}

void Animator::removeState(string name) {
	states.erase(name);
}

string Animator::evaluateState() {
	return stateMachine(*actor, activeStateName);
}

float Animator::getProgress() {
	if(activeStateName.empty())
		return 0;

	return (Time::time - epoch) / states[activeStateName].duration;
}

int Animator::getStep() {
	auto state = states[activeStateName];
	auto count = state.getStepCount();
	auto step = (int)round(count * getProgress());

	return state.loops ? step % count : clamp(step, 0, count - 1);
}

bool Animator::hasState() {
	return !activeStateName.empty();
}

const State& Animator::getActiveState() {
	return states[activeStateName];
}

string Animator::getActiveStateName() const {
	return activeStateName;
}