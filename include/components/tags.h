#pragma once

#include <string>
#include <initializer_list>
#include <unordered_set>
#include "component.h"

/**
 * Tags to distinguish actors with similar components from each other
 */
class Tags : public Component {
	public:
		Tags();
		Tags(std::initializer_list<std::string>);
		void add(std::string);
		void remove(std::string);
		bool has(std::string);
	private:
		std::unordered_set<std::string> value;
};