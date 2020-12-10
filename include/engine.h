#pragma once

#include <functional>
#include <map>
#include <SDL2/SDL.h>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "actor.h"
#include "structures/point.h"
#include "structures/vector2.h"
#include "structures/color.h"
#include "system.h"
#include "utils/property.h"

/**
 * Contains objects that each control a specific flow
 */
class Engine {
	public:
		/** Whether the game is running */
		bool running;

		/**
		 * Setups the window to render
		 * @param name Window name
		 * @param width Width of the game window
		 * @param height Height of the game window
		 */
		void setupDisplay(std::string name, int width, int height);

		/**
		 * Registers an actor in by the engine
		 * @param actor Actor to add
		 */
		void registerActor(Actor *actor);

		/** Handles keyboard and mouse events */
		void handleEvents();

		/** Updates object mechanics */
		void update();

		/** Renders objects */
		void render();

		/** Calculates time properties */
		void clock();

		/**
		 * @return Engine singleton instance
		 */
		static Engine& getInstance();

		operator SDL_Window*() const;
		operator SDL_Renderer*() const;

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		/**
		 * Global input manager
		 */
		class Input {
			public:
				static Property<Vector2> mousePosition;

				/**
				 * @param key Key name
				 * @return Whether the key was pressed this frame
				 */
				static bool down(std::string key);

				/**
				 * @param keyCode Key code
				 * @return Whether the key was pressed this frame
				 */
				static bool down(int keyCode);

				/**
				 * @param key Key name
				 * @return Whether the key is pressed
				 */
				static bool held(std::string key);

				/**
				 * @param keyCode Key code
				 * @return Whether the key is pressed
				 */
				static bool held(int keyCode);

				/**
				 * @param key Key name
				 * @return Whether the key was released this frame
				 */
				static bool up(std::string key);

				/**
				 * 
				 * @param keyCode Key code
				 * @return Whether the key was released this frame
				 */
				static bool up(int keyCode);
			private:
				struct IgnoreCaseComparator {
					bool operator()(const std::string&, const std::string&) const;
				};

				static std::map<std::string, int, IgnoreCaseComparator> keyToCode;
		};

		/** Global temporal properties */
		class Time {
			public:
				/** Frame limit */
				static int targetFps;

				/** Current frames per second */
				static Property<int> fps;

				/** Time since last frame in seconds */
				static Property<float> deltaTime;

				/** Time since the engine was started */
				static Property<float> time;
		};

		/** Used for interaction with the visuals */
		class Display {
			public:
				/** Display center */
				static Vector2 center;

				/** Not meetings */
				static float zoom;

				/** Background color */
				static Color backgroundColor;

				/** Screen resolution to scale to */
				static Point targetResolution;

				/**
				 * @return The width and height of the screen
				 */
				static Point getScreenSize();

				/**
				 * @param screenPoint Point on the screen
				 * @return The point on the screen converted to a point in the world
				 */
				static Vector2 screenToWorld(Vector2 screenPoint);
		};

		/** Enables batch interactions with spawned actors */
		class Actors {
			public:
				/**
				 * Executes the action for all actors containing the specified component types
				 * @tparam T Component types
				 * @param action Action be executed on the matching actors
				 */
				template<typename ...T>
				static void forEach(std::function<void(Actor&)> action) {
					std::vector<Actor*> queried;

					for(auto pair : getInstance().actors)
						if(pair.second->has<T...>())
							queried.push_back(pair.second);

					for(auto actor : queried)
						action(*actor);
				}
		};

		/** All the systems which control updates/renders using the components bound to actors */
		class Systems {
			public:
				/**
				 * Adds a new system
				 * @tparam T System type
				 */
				template<typename T, typename = std::enable_if_t<std::is_base_of<System, T>::value && std::is_default_constructible<T>::value>>
				static void add() {
					getInstance().systems.push_back(new T());
				}
		};
	private:
		bool isDisplayed;
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Event currentEvent;

		/** Actors */
		std::unordered_map<unsigned int, Actor*> actors;

		/** Systems */
		std::vector<System*> systems;

		/* Input */
		std::unordered_set<int> downKeys, heldKeys, upKeys;

		/* Timer */
		int fps = 0;
		float deltaTime = 0;
		float epoch = 0;

		Engine();
		~Engine();
};