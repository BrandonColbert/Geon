#pragma once

/**
 * Uses the component data of actors to create effects
 */
class System {
	public:
		virtual ~System();

		/** Updates qualified actors with their component data */
		virtual void update();

		/** Renders qualified actors with their component data */
		virtual void render();
};