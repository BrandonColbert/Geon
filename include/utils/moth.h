#pragma once

/**
 * Other Math aka M(O)th
 */
namespace Moth {
	/**
	 * @param value Value to get sign of
	 * @return Sign of the value or 0 if 0 
	 */
	template<typename T>
	int sign(T value) {
		return (T(0) < value) - (value < T(0));
	}

	/**
	 * Linearly interpolates between a and b
	 * @param a Starting value when t=0
	 * @param b Ending value when t=1
	 * @param t Interpolation value
	 * @return The interpolate value
	 */
	template<typename T>
	T lerp(T a, T b, T t) {
		return a + (b - a) * t;
	}
};