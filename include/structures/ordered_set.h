#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>

/**
 * A set of ordered values
 */
template<typename T>
class OrderedSet {
	public:
		/**
		 * @param values Initial values
		 * @param comparator Compares values to determine order
		 */
		OrderedSet(std::initializer_list<T> values, std::function<bool(T, T)> comparator) : values(values), comparator(comparator) {}

		/**
		 * @param comparator Compares values to determine order
		 */
		OrderedSet(std::function<bool(T, T)> comparator) : comparator(comparator) {}

		/**
		 * Add a value to the set if it is not already present
		 * @param value Value to add to the set
		 * @return Whether the value was added
		 */
		bool push(T value) {
			if(std::find(values.begin(), values.end(), value) != values.end())
				return false;

			values.emplace_back(value);
			altered = true;

			return true;
		}

		/**
		 * Remove the first element of the set
		 * @return The first element of the set
		 */
		T pop() {
			if(altered) {
				altered = false;
				std::sort(values.begin(), values.end(), comparator);
			}

			auto value = values.begin();
			values.erase(value);

			return *value;
		}

		/**
		 * @return Whether any elements are left in the set
		 */
		bool empty() const {
			return values.size() == 0;
		}

		/**
		 * @return Number of elements in the set
		 */
		std::size_t size() const {
			return values.size();
		}
	private:
		std::vector<T> values;
		std::function<bool(T, T)> comparator;
		bool altered = false;
};