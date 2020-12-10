#include "structures/point.h"

#include <tuple>

using namespace std;

Point::Point() {
	x = 0;
	y = 0;
}

Point::Point(int x, int y) {
	this->x = x;
	this->y = y;
}

Point::Point(const Point &other) {
	x = other.x;
	y = other.y;
}

ostream& operator<<(ostream &stream, const Point &point) {
	stream << "<" << point.x << ", " << point.y << ">";
	return stream;
}

bool Point::operator==(const Point &other) const {
	return x == other.x && y == other.y;
}

bool Point::operator!=(const Point &other) const {
	return x != other.x || y != other.y;
}

bool Point::operator<(const Point &other) const {
	return tie(x, y) < tie(other.x, other.y);
}

Point& Point::operator=(const Point &other) {
	x = other.x;
	y = other.y;

	return *this;
}

Point Point::operator+(const Point &other) const {
	return Point(x + other.x, y + other.y);
}

Point& Point::operator+=(const Point &other) {
	x += other.x;
	y += other.y;
	return *this;
}

Point Point::operator-(const Point &other) const {
	return Point(x - other.x, y - other.y);
}

Point& Point::operator-=(const Point &other) {
	x -= other.x;
	y -= other.y;
	return *this;
}