#include "generation/tilemap.h"

#include <iostream>
#include <sstream>
#include "actor.h"
#include "components/collider.h"
#include "components/rect.h"
#include "components/sprite.h"
#include "structures/texture.h"
#include "utils/format.h"
#include "console.h"

using namespace std;
using namespace Format;
using Tile = Tilemap::Tile;

Tilemap::Tilemap(Grid<Tile> tiles) : tiles(tiles) {}

void Tilemap::actualize() {
	auto halfWidth = tiles.getWidth() / 2;
	auto halfHeight = tiles.getHeight() / 2;

	tiles.forEach([&](auto i, auto j) {
		if(tiles(i, j) == Tile::None)
			return;

		auto &tile = Actor::spawn(format("Tile(%, %)", i, j));

		tile.add(new Rect(
			{(i - halfWidth) * tileSize, (halfHeight - j) * tileSize},
			{tileSize, tileSize}
		));

		switch(tiles(i, j)) {
			case Tile::None:
				tile.add(new Sprite(Texture("./resources/tiles/background.png")));
				break;
			case Tile::Floor:
				tile.add(new Sprite(Texture("./resources/tiles/floor.png")));
				break;
			case Tile::Wall:
				tile.add(new Sprite(Texture("./resources/tiles/wall.png")));
				tile.add(new Collider({tileSize, tileSize}));
				break;
			default:
				break;
		}
	});
}

void Tilemap::visualize() {
	// cout << tiles.toString([](float v) {
	// 	stringstream stream;
	// 	stream << fixed << setprecision(2);
		
	// 	if(v == 0)
	// 		stream << " " << T(0);
	// 	else {
	// 		if(!signbit(v))
	// 			stream << " ";

	// 		stream << v;
	// 	}

	// 	return stream.str();
	// });

	cout << tiles.toString([](auto v) {
		switch(v) {
			case Tile::None:
				return ".";
			case Tile::Floor:
				return "O";
			case Tile::Wall:
				return "X";
			case Tile::Path:
				return "#";
			default:
				return "?";
		}
	});
}