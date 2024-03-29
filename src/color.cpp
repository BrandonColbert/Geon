#include "structures/color.h"

#include "utils/moth.h"

Color Color::red(1, 0, 0);
Color Color::green(0, 1, 0);
Color Color::blue(0, 0, 1);
Color Color::white(1, 1, 1);
Color Color::black(0, 0, 0);

Color::Color(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(float r, float g, float b) : Color(r, g, b, 1) {}

Color::Color(int argb) {
	auto a = (argb >> 24) & 0xff;
	auto r = (argb >> 16) & 0xff;
	auto g = (argb >> 8) & 0xff;
	auto b = argb & 0xff;

	this->a = (float)a / 255.0f;
	this->r = (float)r / 255.0f;
	this->g = (float)g / 255.0f;
	this->b = (float)b / 255.0f;
}

int Color::getRGB() const {
	auto r = (int)(this->r * 255.0f) & 0xff;
	auto g = (int)(this->g * 255.0f) & 0xff;
	auto b = (int)(this->b * 255.0f) & 0xff;

	return (r << 16) | (g << 8) | b;
}

int Color::getRGBA() const {
	auto a = (int)(this->a * 255.0f) & 0xff;
	return (getRGB() << 8) | a;
}

int Color::getARGB() const {
	auto a = (int)(this->a * 255.0f) & 0xff;
	return (a << 24) | getRGB();
}

Color Color::operator+(const Color &other) {
	return Color(r + other.r, g + other.g, b + other.b, a + other.a);
}

Color& Color::operator+=(const Color &other) {
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;

	return *this;
}

Color Color::operator-(const Color &other) {
	return Color(r - other.r, g - other.g, b - other.b, a - other.a);
}

Color& Color::operator-=(const Color &other) {
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;

	return *this;
}

Color Color::operator*(const Color &other) {
	return Color(r * other.r, g * other.g, b * other.b, a * other.a);
}

Color& Color::operator*=(const Color &other) {
	r *= other.r;
	g *= other.g;
	b *= other.b;
	a *= other.a;

	return *this;
}

Color Color::operator/(const Color &other) {
	return Color(r / other.r, g / other.g, b / other.b, a / other.a);
}

Color& Color::operator/=(const Color &other) {
	r /= other.r;
	g /= other.g;
	b /= other.b;
	a /= other.a;

	return *this;
}

Color Color::operator*(const float value) {
	return Color(r * value, g * value, b * value, a * value);
}

Color& Color::operator*=(const float value) {
	r *= value;
	g *= value;
	b *= value;
	a *= value;

	return *this;
}

Color operator*(float value, const Color &color) {
	return Color(color.r * value, color.g * value, color.b * value, color.a * value);
}

Color Color::operator/(const float value) {
	return Color(r / value, g / value, b / value, a / value);
}

Color& Color::operator/=(const float value) {
	r /= value;
	g /= value;
	b /= value;
	a /= value;

	return *this;
}

Color::operator SDL_Color() const {
	return SDL_Color{
		(Uint8)(r * 255),
		(Uint8)(g * 255),
		(Uint8)(b * 255),
		(Uint8)(a * 255)
	};
}

Color Color::lerp(Color a, Color b, float t) {
	return Color(
		Moth::lerp(a.r, b.r, t),
		Moth::lerp(a.g, b.g, t),
		Moth::lerp(a.b, b.b, t),
		Moth::lerp(a.a, b.a, t)
	);
}