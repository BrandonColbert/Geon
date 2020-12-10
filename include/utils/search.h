#pragma once

#include <algorithm>
#include <functional>
#include <limits>
#include <map>
#include <vector>
#include "structures/grid.h"
#include "structures/ordered_set.h"
#include "structures/point.h"

using Cost = std::function<float(Point, Point)>;

namespace Search {
	namespace Heuristic {
		float manhattanDistance(Point start, Point end);
		float euclideanDistance(Point start, Point end);
	}

	/**
	 * @param grid Grid to find the path on
	 * @param start Start space
	 * @param goal Goal space
	 * @param accessible Whether the space can be passed through to reach the goal 
	 * @param estimateCost Estimated cost to reach the goal from a space
	 * @param cost Weight of the edge from current to neighbor
	 * @return The sequence of points to get to start to end
	 * 
	 * https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
	 */
	template<typename T>
	std::vector<Point> astar(Grid<T> grid, Point start, Point goal, std::function<bool(Point)> accessible, Cost estimateCost = Heuristic::manhattanDistance, Cost cost = [](auto, auto){return 1;}) {
		using namespace std;

		map<Point, Point> from;
		map<Point, float> gScore{{start, 0}};
		map<Point, float> fScore{{start, estimateCost(start, goal)}};

		auto getGScore = [&](Point p) {
			return gScore.find(p) != gScore.end() ? gScore[p] : numeric_limits<float>::max();
		};

		auto getFScore = [&](Point p) {
			return fScore.find(p) != fScore.end() ? fScore[p] : numeric_limits<float>::max();
		};

		OrderedSet<Point> openSet({start}, [&](Point left, Point right) {
			return getFScore(left) < getFScore(right);
		});

		while(!openSet.empty()) {
			auto current = openSet.pop();

			if(current == goal) {
				vector<Point> path{current};

				while(from.find(current) != from.end()) {
					current = from[current];
					path.push_back(current);
				}

				reverse(path.begin(), path.end());

				return path;
			}

			for(auto neighbor : grid.getNeighbors(current)) {
				if(!accessible(neighbor))
					continue;

				auto tentativeGScore = getGScore(current) + cost(current, neighbor);

				if(tentativeGScore < getGScore(neighbor)) {
					from[neighbor] = current;
					gScore[neighbor] = tentativeGScore;
					fScore[neighbor] = getGScore(neighbor) + estimateCost(neighbor, goal);

					openSet.push(neighbor);
				}
			}
		}

		return vector<Point>();
	}
}