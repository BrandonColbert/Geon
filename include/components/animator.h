#pragma once

#include <string>
#include <unordered_map>
#include "animation/state.h"
#include "component.h"

/**
 * Allows an object to be animated
 */
class Animator : public Component {
	public:
		/**
		 * Defines a new animatable state
		 * @param state State to add
		 */
		void defineState(State state);

		/**
		 * Removes the state by name
		 * @param name Name of state to remove
		 */
		void removeState(std::string name);

		/**
		 * Transitions to the state by name
		 * @param state Name of the state
		 */
		void transition(std::string state = "");

		/**
		 * @return Progress of the current state
		 */
		float getProgress();

		/**
		 * @return Current step in the state 
		 */
		int getStep();

		/**
		 * @return Whether the animator currently has a state
		 */
		bool hasState();

		/**
		 * @return Current state
		 */
		const State& getActiveState();
	private:
		std::unordered_map<std::string, State> states;
		std::string activeStateName;
		float epoch;
};