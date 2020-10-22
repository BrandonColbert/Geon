#pragma once

#include <iostream>
#include "utils/format.h"

/**
 * Utility methods relating to the terminal io
 */
namespace Console {
	/**
	 * Prints a formatted message to standard output
	 * @param message Message to print
	 * @param args Values to inject
	 */
	template<typename ...Ts>
	void print(const char *message, Ts ...args) {
		Format::formatTo(std::cout, message, args...);
		std::cout << std::endl;
	}

	/**
	 * Prints a formatted message to standard error
	 * @param message Message to print
	 * @param args Values to inject
	 */
	template<typename ...Ts>
	void error(const char *message, Ts ...args) {
		Format::formatTo(std::cerr, message, args...);
		std::cerr << std::endl;
	}

	/**
	 * Prints a formatted message to standard log
	 * @param message Message to print
	 * @param args Values to inject
	 */
	template<typename ...Ts>
	void log(const char *message, Ts ...args) {
		Format::formatTo(std::clog, message, args...);
		std::clog << std::endl;
	}
}