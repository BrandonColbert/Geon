#include "math/vector2.h"

#include <cmath>

using namespace std;

Vector2 Vector2::zero(0, 0);
Vector2 Vector2::right(1, 0);
Vector2 Vector2::left(-1, 0);
Vector2 Vector2::up(0, 1);
Vector2 Vector2::down(0, -1);
Vector2 Vector2::one(1, 1);

Vector2::Vector2() {
	x = 0;
	y = 0;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const tuple<float, float> &position) {
	auto components = position;
	x = get<0>(components);
	y = get<1>(components);
}

Vector2::Vector2(const Vector2 &other) {
	x = other.x;
	y = other.y;
}

float Vector2::sqr_magnitude() {
	return x * x + y * y;
}

float Vector2::magnitude() {
	return sqrt(sqr_magnitude());
}

Vector2 Vector2::normalized() {
	return *this / magnitude();
}

Vector2::operator tuple<float, float>() {
	return make_tuple(x, y);
}

ostream& operator<<(ostream &stream, const Vector2 &vector) {
	stream << "<" << vector.x << ", " << vector.y << ">";
	return stream;
}

bool Vector2::operator==(const Vector2 &other) const {
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2 &other) const {
	return x != other.x || y != other.y;
}

Vector2& Vector2::operator=(const Vector2 &other) {
	x = other.x;
	y = other.y;

	return *this;
}

Vector2 Vector2::operator+(const Vector2 &other) {
	return Vector2(x + other.x, y + other.y);
}

Vector2& Vector2::operator+=(const Vector2 &other) {
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2 &other) {
	return Vector2(x - other.x, y - other.y);
}

Vector2& Vector2::operator-=(const Vector2 &other) {
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2 Vector2::operator*(float value) {
	return Vector2(x * value, y * value);
}

Vector2& Vector2::operator*=(float value) {
	x *= value;
	y *= value;
	return *this;
}

Vector2 operator*(float scalar, const Vector2 &vector) {
	return Vector2(vector.x * scalar, vector.y * scalar);
}

Vector2 Vector2::operator/(float value) {
	return Vector2(x / value, y / value);
}

Vector2& Vector2::operator/=(float value) {
	x /= value;
	y /= value;
	return *this;
}
