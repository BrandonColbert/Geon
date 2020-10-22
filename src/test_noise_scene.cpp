#include "scenes/test_noise_scene.h"

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "generation/value_noise.h"

using namespace std;

void TestNoiseScene::create() {
	ValueNoise noise(100, 100, 0);
	auto grid = noise.toGrid();

	cout << grid.toString([](float v) {
		stringstream stream;
		stream << fixed << setprecision(3);
		
		if(v >= 0)
			stream << " ";

		stream << v;

		return stream.str();
	});

	// grid.writeImage("noise", [](auto v) {
	// 	return (unsigned char)(v * 255);
	// });
}