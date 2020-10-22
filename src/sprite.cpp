#include "components/sprite.h"

Sprite::Sprite(Texture texture, float depth) : texture(texture) {
	size.x = texture.getWidth();
	size.y = texture.getHeight();
	this->depth = depth;
}

Sprite::Sprite(float depth) {
	this->depth = depth;
}

Sprite::Sprite() {
	depth = 0;
}