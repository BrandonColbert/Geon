#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "structures/state.h"
#include "component.h"

using StateMachine = std::function<std::string(Actor&, std::string)>;

/**
 * Allows an object to be animated
 */
class Animator : public Component {
	public:
		/**
		 * @param stateMachine Returns the current state that the actor should be in based on the actor and current state
		 */
		Animator(StateMachine stateMachine);

		Animator();

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
		 * @return State the actor should be in according to state machine
		 */
		std::string evaluateState();

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

		/**
		 * @return Name of the current state
		 */
		std::string getActiveStateName() const;
	private:
		std::unordered_map<std::string, State> states;
		std::string activeStateName;
		StateMachine stateMachine;
		float epoch;
};