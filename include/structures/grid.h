#pragma once

#include <cmath>
#include <initializer_list>
#include <fstream>
#include <functional>
#include <ostream>
#include <string>
#include <sstream>
#include <tuple>
#include <vector>
#include "utils/format.h"

/**
 * Represents a 2d collection of elements
 */
template<typename T>
class Grid {
	public:
		/**
		 * @param width Grid width
		 * @param height Grid height
		 */
		Grid(int width, int height) : elements(width * height) {
			this->width = width;
			this->height = height;
		}

		/**
		 * @param width Grid width
		 * @param height Grid height
		 * @param defaultElement Default element to fill empty spaces with
		 */
		Grid(int width, int height, T defaultElement) : elements(width * height, defaultElement) {
			this->width = width;
			this->height = height;
		}

		/**
		 * @param width Grid width
		 * @param height Grid height
		 * @param func Populator function
		 */
		Grid(int width, int height, std::function<T(int, int)> func) {
			this->width = width;
			this->height = height;

			elements.reserve(width, height);			
			for(auto j = 0; j < height; j++)
				for(auto i = 0; i < width; i++)
					elements.push_back(func(i, j));
		}

		/**
		 * @param elements 2d array to use as elements
		 */
		Grid(std::initializer_list<std::initializer_list<T>> elements) {
			if(elements.size() == 0) {
				width = height = 0;
				return;
			}

			width = elements.begin()->size();
			height = elements.size();

			this->elements.reserve(width * height);
			for(auto row : elements)
				for(auto element : row)
					this->elements.push_back(element);

			if((int)this->elements.size() != width * height)
				throw std::runtime_error(Format::format(
					"Grid %x% has %/% elements",
					width, height, this->elements.size(), width * height
				));
		}

		/**
		 * @param other Grid to copy dimensions and elements from
		 */
		Grid(const Grid<T> &other) : elements(other.elements) {
			width = other.width;
			height = other.height;
		}

		Grid() : Grid(0, 0) {}

		/**
		 * @return Grid width 
		 */
		int getWidth() const {
			return width;
		}

		/**
		 * @return Grid height 
		 */
		int getHeight() const {
			return height;
		}

		/**
		 * Executes the function on every grid space
		 * @param func Called for each grid space
		 */
		void forEach(std::function<void(int, int)> func) {
			for(auto j = 0; j < height; j++)
				for(auto i = 0; i < width; i++)
					func(i, j);
		}

		/**
		 * Provides each grid space with a value
		 * @param func Provides a value for each grid space
		 */
		void populate(std::function<T(int, int)> func) {
			for(auto j = 0; j < height; j++)
				for(auto i = 0; i < width; i++)
					elements[j * width + i] = func(i, j);
		}

		/**
		 * Provides elements for a new grid then updates this grid to the new grid
		 * @param func Provides a value for each grid space
		 */
		void update(std::function<T(int, int)> func) {
			std::vector<T> elements(width * height);

			for(auto j = 0; j < height; j++)
				for(auto i = 0; i < width; i++)
					elements[j * width + i] = func(i, j);

			this->elements = elements;
		}

		/**
		 * @return Whether the tile is inside the map
		 */
		bool inside(int x, int y) {
			if(x < 0 || width <= x)
				return false;
			if(y < 0 || height <= y)
				return false;

			return true;
		}

		/**
		 * @param x Space x-coordinate
		 * @param y Space y-coordinate
		 * @param radius Radius in which to acquire neighbors
		 * @return Grid spaces within the given radius of the specified space
		 */
		std::vector<std::tuple<int, int>> getNeighbors(int x, int y, float radius = 1) {
			std::vector<std::tuple<int, int>> neighbors;

			auto v = (int)std::ceil(radius);
			auto r2 = radius * radius;

			for(auto j = -v; j <= v; j++)
				for(auto i = -v; i <= v; i++)
					if(inside(x + i, y + j) && (i * i + j * j) <= r2 && !(i == 0 && j == 0))
						neighbors.push_back(std::make_tuple(x + i, y + j));

			return neighbors;
		}

		/**
		 * @param space Space coordinate
		 * @param radius Radius in which to acquire neighbors
		 * @return Grid spaces within the given radius of the specified space
		 */
		std::vector<std::tuple<int, int>> getNeighbors(std::tuple<int, int> space, float radius = 1) {
			return getNeighbors(std::get<0>(space), std::get<1>(space), radius);
		}

		/**
		 * @param valueOf Converts an element to a string representation
		 * @return A formatted string representing this grid's spaces
		 */
		std::string toString(std::function<std::string(T)> valueOf) const {
			std::stringstream stream;

			for(auto j = 0; j < height; j++) {
				stream << valueOf(elements[j * width]);

				for(auto i = 1; i < width; i++)
					stream << " " << valueOf(elements[j * width + i]);

				stream << std::endl;
			}

			return stream.str();
		}

		/**
		 * Creates a PPM file of the grid
		 * @param path File path
		 * @param converter Converts an element to a pixel representation
		 */
		void writeImage(std::string path, std::function<unsigned char(T)> converter) {
			std::ofstream stream;
			stream.open(Format::format("%.ppm", path), std::ios::out | std::ios::binary);

			stream << "P6\n" << width << " " << height << "\n255\n";

			for(auto i = 0; i < width * height; i++) {
				auto v = converter(elements[i]);
				stream << v << v << v;
			}

			stream.close();
		}

		T& operator()(int x, int y) {
			return elements[y * width + x];
		}

		T& operator()(std::tuple<int, int> p) {
			return elements[std::get<1>(p) * width + std::get<0>(p)];
		}

		T& operator[](int index) {
			return elements[index];
		}

		Grid<T>& operator=(const Grid<T> &other) {
			width = other.width;
			height = other.height;
			elements = other.elements;

			return *this;
		}

		bool operator==(const Grid<T> &other) {
			return elements == other.elements;
		}

		bool operator!=(const Grid<T> &other) {
			return elements != other.elements;
		}

		friend std::ostream& operator<<(std::ostream &stream, const Grid<T> &grid) {
			stream << grid.toString([](float v) {
				std::stringstream stream;
				stream << v;

				return stream.str();
			});

			return stream;
		}
	private:
		int width, height;
		std::vector<T> elements;
};