#pragma once

#include <tuple>
#include <ostream>

/**
 * Represents a 2-dimensional vector
 */
class Vector2 {
	public:
		static Vector2 zero, right, left, up, down, one;

		float x, y;

		Vector2();
		Vector2(float x, float y);
		Vector2(const std::tuple<float, float> &position);
		Vector2(const Vector2&);

		float sqr_magnitude();
		float magnitude();
		Vector2 normalized();

		operator std::tuple<float, float>();
		friend std::ostream& operator<<(std::ostream &stream, const Vector2 &vector);

		bool operator==(const Vector2&) const;
 		bool operator!=(const Vector2&) const;

		Vector2& operator=(const Vector2&);

		Vector2 operator+(const Vector2 &other);
		Vector2& operator+=(const Vector2 &other);

		Vector2 operator-(const Vector2 &other);
		Vector2& operator-=(const Vector2 &other);

		Vector2 operator*(float value);
		Vector2& operator*=(float value);
		friend Vector2 operator*(float scalar, const Vector2 &vector);

		Vector2 operator/(float value);
		Vector2& operator/=(float value);
};