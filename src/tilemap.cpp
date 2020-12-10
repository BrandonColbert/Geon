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

Tilemap::Tilemap() {}

Vector2 Tilemap::tileToWorld(Point point) {
	auto halfWidth = tiles.getWidth() / 2;
	auto halfHeight = tiles.getHeight() / 2;

	return {(point.x - halfWidth) * tileSize, (halfHeight - point.y) * tileSize};
}

void Tilemap::actualize() {
	tiles.forEach([&](int i, int j) {
		if(tiles(i, j) == Tile::None)
			return;

		auto &tile = Actor::spawn(format("Tile(%, %)", i, j));

		tile.add(new Rect(
			tileToWorld(Point(i, j)),
			{tileSize, tileSize}
		));

		switch(tiles(i, j)) {
			case Tile::None:
				tile.add(new Sprite(Texture("./resources/tiles/background.png")));
				break;
			case Tile::Floor:
				tile.add(new Sprite(Texture("./resources/tiles/floor.png")));
				break;
			case Tile::Path:
				tile.add(new Sprite(Texture("./resources/tiles/floor.png")));
				tile.get<Sprite>().tint = Color(0.9, 0.9, 0.9);
				break;
			case Tile::Wall:
				tile.add(new Sprite(Texture("./resources/tiles/wall.png")));
				tile.add(new Collider({tileSize, tileSize}));
				break;
			case Tile::Plank:
				tile.add(new Sprite(Texture("./resources/tiles/plank.png")));
				tile.add(new Collider({tileSize, tileSize}));
				break;
			default:
				break;
		}
	});
}

void Tilemap::visualize() {
	cout << tiles.toString([](Tile v) {
		switch(v) {
			case Tile::None:
				return ".";
			case Tile::Floor:
				return "o";
			case Tile::Wall:
				return "-";
			case Tile::Path:
				return "#";
			default:
				return "?";
		}
	});
}

Grid<Tile>& Tilemap::getTiles() {
	return tiles;
}