#include "components/mob.h"

#include "console.h"
#include "systems/mob_system.h"

using namespace std;

Mob::Mob(string type) {
	setType(type);
}

string Mob::getType() {
	return type;
}

void Mob::setType(string value) {
	if(!MobSystem::typeExists(value)) {
		Console::error("Mob type '%' does not exist");
		return;
	}

	type = value;
}