#pragma once

#include <ostream>
#include "structures/point.h"

/**
 * Represents a 2-dimensional vector
 */
class Vector2 {
	public:
		static Vector2 zero, right, left, up, down, one;

		float x, y;

		Vector2();
		Vector2(float x, float y);
		Vector2(const Point &position);
		Vector2(const Vector2&);

		float sqrMagnitude();
		float magnitude();
		Vector2 normalized();

		/**
		 * @param other Vector to multiply component wise
		 * @return Component wise multiplication of this vector and the given
		 */
		Vector2 scale(Vector2 other);

		/**
		 * @return A new vector containing with components as the absolute value of this vector's components
		 */
		Vector2 abs();

		friend std::ostream& operator<<(std::ostream&, const Vector2&);

		bool operator==(const Vector2&) const;
 		bool operator!=(const Vector2&) const;

		Vector2& operator=(const Vector2&);
		Vector2 operator-() const;

		Vector2 operator+(const Vector2&) const;
		Vector2& operator+=(const Vector2&);

		Vector2 operator-(const Vector2&) const;
		Vector2& operator-=(const Vector2&);

		Vector2 operator*(float) const;
		Vector2& operator*=(float);
		friend Vector2 operator*(float, const Vector2&);

		Vector2 operator/(float) const;
		Vector2& operator/=(float);
		friend Vector2 operator/(float, const Vector2&);

		operator Point() const;
};