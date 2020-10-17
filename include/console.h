#pragma once

#include <iostream>
#include "utils/printer.h"

namespace Console {
	/**
	 * Prints a formatted message to standard output
	 * @param message Message to print
	 * @param args Values to inject
	 */
	template<typename ...Ts>
	void print(const char *message, Ts ...args) {
		Printer::printTo(std::cout, message, args...);
		std::cout << std::endl;
	}

	/**
	 * Prints a formatted message to standard error
	 * @param message Message to print
	 * @param args Values to inject
	 */
	template<typename ...Ts>
	void error(const char *message, Ts ...args) {
		Printer::printTo(std::cerr, message, args...);
		std::cerr << std::endl;
	}

	template<typename ...Ts>
	void log(const char *message, Ts ...args) {
		Printer::printTo(std::clog, message, args...);
		std::clog << std::endl;
	}
}