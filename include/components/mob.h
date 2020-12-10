#pragma once

#include <string>
#include "component.h"

class Mob : public Component {
	public:
		/**
		 * @param type Mob type to determine behavior and make assumptions
		 */
		Mob(std::string type);

		/**
		 * @return Type of this mob
		 */
		std::string getType();

		/**
		 * Set this mob's type
		 * 
		 * Throws an error if non-existent type is used
		 */
		void setType(std::string);
	private:
		std::string type;
};