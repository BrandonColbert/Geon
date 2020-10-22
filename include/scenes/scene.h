#pragma once

/**
 * Does the setup to create the game being played
 */
class Scene {
	public:
		/**
		 * Start the game
		 */
		void start();
	protected:
		/**
		 * Adds for specific game
		 */
		virtual void create() = 0;
};