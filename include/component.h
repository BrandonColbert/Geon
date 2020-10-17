#pragma once

#include "actor.h"

class Actor;

/**
 * Contains data about and enables interactions with an actor
 */
class Component {
	public:
		virtual ~Component();

		/**
		 * Called when this component is added
		 * @param actor Actor this component was added to
		 */
		virtual void added(Actor &actor);

		/**
		 * Called when this component is removed
		 * @param actor Actor this component was removed from
		 */
		virtual void removed(Actor &actor);

		/** Removes this component from the actor */
		void destroy();

		/** Whether the component is attached to an actor */
		bool exists();
	protected:
		Actor *actor;
	private:
		bool _exists = true;
};