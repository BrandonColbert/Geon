#include "actor.h"

#include "engine.h"
#include "console.h"

using std::string, std::queue;

queue<unsigned int> Actor::recycle;
unsigned int Actor::nextId;

Actor::Actor(unsigned int id, string name) : id(id) {
	this->name = name;
}

Actor::~Actor() {
	for(const auto &entry : components) {
		entry.second->removed(*this);
		delete entry.second;
	}

	recycle.push(id);
}

void Actor::add(Component *component) {
	if(components.find(typeid(*component)) != components.end()) {
		Console::error("Component '%' already exists on '%'", typeid(*component).name(), name);
		return;
	}

	component->added(*this);
	components.emplace(typeid(*component), component);
}

bool Actor::exists() {
	return _exists;
}

void Actor::destroy() {
	_exists = false;
}

void Actor::clean() {
	auto next = components.cbegin();

	while(next != components.cend()) {
		auto type = next->first;
		auto component = next->second;
		next++;

		if(!component->exists()) {
			components.erase(type);
			delete component;
		}
	}
}

Actor& Actor::spawn(string name) {
	auto id = nextId;

	if(recycle.size() > 0) {
		id = recycle.front();
		recycle.pop();
	} else
		nextId++;

	auto actor = new Actor(id, name);
	actor->_exists = true;

	Engine::getInstance().registerActor(actor);

	return *actor;
}

bool Actor::operator==(const Actor &other) {
	return id == other.id;
}

bool Actor::operator!=(const Actor &other) {
	return id != other.id;
}