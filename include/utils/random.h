#pragma once

#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <vector>

/**
 * Utilities relating to random number generation
 */
namespace Random {
	/**
	 * @return A random number of the specified type within [0, 1)
	 */
	template<typename T = float>
	T range() {
		return (T)std::rand() / (T)RAND_MAX;
	}

	/**
	 * @param min Lower bound (inclusive)
	 * @param max Upper bound (exclusive)
	 * @return A random number of the specified type within the given range
	 */
	template<typename T = float>
	T range(T min, T max) {
		return min + (T)std::rand() / ((T)RAND_MAX / (max - min));
	}

	/**
	 * Sequential random numbers from a seed
	 */
	class Sequence {
		public:
			/**
			 * @param seed Seed to create sequence from
			 */
			Sequence(unsigned int seed);

			/**
			 * @return Current seed 
			 */
			int getSeed() {
				return value;
			}

			/**
			 * @return Next number of the sequence
			 */
			template<typename T = float>
			T next() {
				moveNext();
				return value;
			}

			/**
			 * @param count Amount of numbers to take from the sequence
			 * @return Takes numbers from the sequence
			 */
			template<typename T = float>
			std::vector<T> take(int count) {
				std::vector<T> v;
				v.reserve(count);

				for(auto i = 0; i < count; i++)
					v.push_back(next());

				return v;
			}

			/**
			 * @param min Lower bound
			 * @param max Upper bound
			 * @return Next number in the sequence conformed to the given range
			 */
			template<typename T>
			T next(T min, T max);

			/**
			 * @param count Amount of numbers to take from the sequence
			 * @param min Lower bound
			 * @param max Upper bound
			 * @return Takes numbers from the sequence and conforms them to the given range
			 */
			template<typename T>
			std::vector<T> take(int count, T min, T max) {
				std::vector<T> v;
				v.reserve(count);

				for(auto i = 0; i < count; i++)
					v.push_back(next(min, max));

				return v;
			}
		protected:
			void moveNext();

			template<typename T>
			T nextRational(T min, T max) {
				return min + next<T>() / T(std::numeric_limits<int>::max()) * (max - min);
			}

			template<typename T>
			T nextInteger(T min, T max) {
				return min + next<T>() % (T(1) + max - min);
			}
		private:
			int value;
	};
}