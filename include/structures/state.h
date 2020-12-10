#pragma once

#include <string>
#include <vector>
#include "structures/point.h"

class State {
	public:
		/** Duration of the state in seconds */
		float duration;

		/** Whether this state loops */
		bool loops;

		/**
		 * @param name State name
		 * @param duration Duration in seconds
		 * @param loops Whether animation loops
		 * @param steps Step to sprite coordinate map
		 */
		State(std::string name, float duration, std::vector<Point> steps = {}, bool loops = false);

		/**
		 * @param name State name
		 * @param duration Duration in seconds
		 * @param loops Whether animation loops
		 * @param start Coordinate of the starting sprite in the sheet
		 * @param range Number of sprites in the animation
		 */
		State(std::string name, float duration, Point start, int count, bool loops = false);

		/**
		 * @param name State name
		 * @param duration Duration in seconds
		 * @param loops Whether animation loops
		 * @param start Y-coordinate of the starting sprite in the sheet
		 * @param range Number of sprites in the animation
		 */
		State(std::string name, float duration, int y, int count, bool loops = false);

		State();

		/** Unique name for this state */
		std::string getName() const;

		/**
		 * @param step Animation step index
		 * @return Coordinates of the step in the sprite sheet
		 */
		Point getCoords(int step) const;

		/**
		 * @return Number of steps in the animation 
		 */
		int getStepCount() const;
	private:
		std::string name;
		std::vector<Point> steps;
};