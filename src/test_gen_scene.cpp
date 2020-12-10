#include "scenes/test_gen_scene.h"

#include <iostream>
#include "generation/floor.h"
#include "utils/random.h"

using namespace std;
using Random::Sequence;

void TestGenScene::create() {
	Floor floor(30, 30, 1);
	floor.rooms = 10;
	floor.distribution = 15;
	floor.spacing = 2;
	floor.minRoomSideLength = 4;
	floor.maxRoomSideLength = 8;

	cout << endl;
	auto info = floor.generate();

	info.tilemap.visualize();
}