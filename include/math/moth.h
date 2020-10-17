#pragma once

/**
 * Other Math
 */
class Moth {
	public:
		/**
		 * @param value Value to get sign of
		 * @return Sign of the value or 0 if 0 
		 */
		template<typename T>
		static int sign(T value) {
			return (T(0) < value) - (value < T(0));
		}
};