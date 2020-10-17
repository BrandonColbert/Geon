#include "component.h"

#include <iostream>

Component::~Component() {}

void Component::added(Actor &actor) {
	this->actor = &actor;
}

void Component::removed(Actor &actor) {
	this->actor = nullptr;
}

void Component::destroy() {
	_exists = false;
}

bool Component::exists() {
	return _exists;
}