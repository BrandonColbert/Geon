#pragma once

#include <ostream>
#include <sstream>
#include <string>

/**
 * Allows objects to be formatted into strings
 */
namespace Format {
	/**
	 * Prints a formatted message to the stream
	 * @param stream Stream to output to
	 * @param message Message to format
	 */
	void formatTo(std::ostream &stream, const char *message);

	/**
	 * Prints a formatted message to the stream
	 * @param stream Stream to output to
	 * @param message Message to format
	 * @param value First argument
	 * @param rest Rest of the arguments
	 */
	template<typename T, typename ...Ts>
	void formatTo(std::ostream &stream, const char *message, T value, Ts ...rest) {
		auto next = message[0];

		switch(next) {
			case '\0':
				stream << std::endl;
				break;
			case '%':
				switch(message[1]) {
					case '%':
						stream << next;
						formatTo(stream, message + 2, value, rest...);
						break;
					default:
						stream << value;
						formatTo(stream, message + 1, rest...);
						break;
				}
				break;
			default:
				stream << next;
				formatTo(stream, message + 1, value, rest...);
				break;
		}
	}

	/**
	 * @param message Message to format
	 * @param args Values to inject
	 * @return Formatted message
	 */
	template<typename ...Ts>
	std::string format(const char *message, Ts ...args) {
		std::stringstream stream;
		formatTo(stream, message, args...);

		return stream.str();
	}
}