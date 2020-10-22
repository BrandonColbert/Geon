#include "scenes/test_tilemap_scene.h"

#include "generation/tilemap.h"
#include "structures/grid.h"

using namespace std;
using Tile = Tilemap::Tile;

void TestTilemapScene::create() {
	const auto n = Tile::None;
	const auto f = Tile::Floor;
	const auto w = Tile::Wall;

	Tilemap floor(Grid<Tile>{
		{n, n, n, n, n, n, n, n, n, n, n, n, n},
		{n, w, w, w, w, w, w, w, w, w, w, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, f, f, f, f, f, f, f, f, f, w, n},
		{n, w, w, w, w, w, w, w, w, w, w, w, n},
		{n, n, n, n, n, n, n, n, n, n, n, n, n}
	});

	floor.visualize();

	floor.tileSize = 50;
	floor.actualize();
}