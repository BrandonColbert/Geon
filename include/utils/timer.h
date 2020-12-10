#pragma once

#include <functional>

class Timer {
	using Callback = std::function<void()>;

	public:
		Callback callback;
		Timer() = delete;

		/**
		 * @param time Wait time
		 * @param callback Called after wait time elapses
		 */
		static void after(float time, Callback callback);
	private:
		Timer(Callback);
};