#include "scenes/test_gen_scene.h"

#include <iostream>
#include "generation/floor.h"
#include "utils/random.h"

using namespace std;
using Random::Sequence;

void TestGenScene::create() {
	Floor floor(50, 50, 0);
	floor.roomCount = 7;
	floor.randomness = 4;
	floor.minRoomSideLength = 5;
	floor.maxRoomSideLength = 8;
	floor.generate();

	// Sequence s(0);

	// for(auto i = 0; i < 5; i++) {
	// 	Floor floor(15, 15, s.next());
	// 	floor.sparsity = 3;
	// 	floor.generate();
	// }
}