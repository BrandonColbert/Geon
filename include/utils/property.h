#pragma once

#include <exception>
#include <iostream>
#include <functional>

/**
 * Simulates getter/setter properties
 */
template<typename T>
class Property {
	using Getter = std::function<T()>;
	using Setter = std::function<void(const T&)>;

	public:
		Property(Getter getter) : getter(getter) {
			setter = [](const T&) {
				throw std::runtime_error("Unable to set on property");
			};
		}

		Property(Setter setter) : setter(setter) {
			getter = []() {
				throw std::runtime_error("Unable to get on property");
			};
		}

		Property(Getter getter, Setter setter) : getter(getter), setter(setter) {}

		void operator=(const T &value) {
			setter(value);
		}

		operator T() const {
			return getter();
		}
	private:
		Getter getter;
		Setter setter;
};