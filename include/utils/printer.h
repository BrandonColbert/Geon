#pragma once

#include <ostream>

namespace Printer {
	/**
	 * Prints a formatted message to the stream
	 * @param stream Stream to output to
	 * @param message Message to print
	 */
	void printTo(std::ostream &stream, const char *message);

	/**
	 * Prints a formatted message to the stream
	 * @param stream Stream to output to
	 * @param message Message to print
	 * @param value First argument
	 * @param rest Rest of the arguments
	 */
	template<typename T, typename ...Ts>
	void printTo(std::ostream &stream, const char *message, T value, Ts ...rest) {
		auto next = message[0];

		switch(next) {
			case '\0':
				stream << std::endl;
				break;
			case '%':
				switch(message[1]) {
					case '%':
						stream << next;
						printTo(stream, message + 2, value, rest...);
						break;
					default:
						stream << value;
						printTo(stream, message + 1, rest...);
						break;
				}
				break;
			default:
				stream << next;
				printTo(stream, message + 1, value, rest...);
				break;
		}
	}
}