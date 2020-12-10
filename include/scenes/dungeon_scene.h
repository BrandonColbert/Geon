#pragma once

#include "scenes/scene.h"
#include "generation/tilemap.h"
#include "structures/grid.h"

/**
 * geon that is not Dun
 */
class DungeonScene : public Scene {
	public:
		static Tilemap tilemap;
		static int remainingMobs;
	protected:
		void create() override;
};