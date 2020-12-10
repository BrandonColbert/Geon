#pragma once

#include <ostream>

class Point {
	public:
		int x, y;

		Point();
		Point(int x, int y);
		Point(const Point&);

		friend std::ostream& operator<<(std::ostream &stream, const Point &point);

		bool operator==(const Point&) const;
 		bool operator!=(const Point&) const;
		bool operator<(const Point&) const;

		Point& operator=(const Point&);

		Point operator+(const Point &other) const;
		Point& operator+=(const Point &other);

		Point operator-(const Point &other) const;
		Point& operator-=(const Point &other);
};