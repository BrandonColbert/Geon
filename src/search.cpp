#include "utils/search.h"

#include <cmath>

using namespace std;
using namespace Search;

float Heuristic::manhattanDistance(Point start, Point end) {
	return abs(start.x - end.x) + abs(start.y - end.y);
}

float Heuristic::euclideanDistance(Point start, Point end) {
	return sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
}