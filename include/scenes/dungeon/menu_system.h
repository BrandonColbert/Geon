#pragma once

#include "system.h"

enum Menu {
	Ingame,
	Main,
	Pause,
	Death
};

class MenuSystem : public System {
	public:
		static Menu currentMenu;

		void update() override;
		void render() override;
};