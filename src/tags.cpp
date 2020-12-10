#include "components/tags.h"

using namespace std;

Tags::Tags() {}
Tags::Tags(initializer_list<string> list) : value(list) {}

void Tags::add(string tag) {
	value.insert(tag);
}

void Tags::remove(string tag) {
	value.erase(tag);
}

bool Tags::has(string tag) {
	return value.find(tag) != value.end();
}