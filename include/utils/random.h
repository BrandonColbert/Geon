#pragma once

#include <cstdlib>
#include <ctime>

namespace Random {
	/**
	 * @return A random number of the specified type
	 */
	template<typename T = float>
	T next() {
		return (T)std::rand() / (T)RAND_MAX;
	}

	/**
	 * @param lower Lower bound (inclusive)
	 * @param upper Upper bound (exclusive)
	 * @return A random number of the specified type within the given range
	 */
	template<typename T = float>
	T range(T lower, T upper) {
		return lower + (T)std::rand() / ((T)RAND_MAX / (upper - lower));
	}
}