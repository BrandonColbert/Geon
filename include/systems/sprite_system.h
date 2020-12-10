#pragma once

#include "system.h"

class SpriteSystem : public System {
	public:
		/** Coefficient to cull out of bounds sprites with */
		static float EdgeOverflow;
		void render() override;
};