#pragma once

#include <string>
#include <typeindex>
// #include <type_traits>
#include <unordered_map>
#include <queue>
#include "component.h"

class Component;

/**
 * Some object that does something in the game
 */
class Actor {
	public:
		/** Identifies this actor */
		const unsigned int id;

		/** Name of this actor */
		std::string name;

		virtual ~Actor();

		/**
		 * Adds the component to this actor
		 * 
		 * Fails if a component of the same type already exists
		 * @param component Component to add
		 */
		void add(Component *component);

		/**
		 * Removes the component by type
		 * @tparam T Component type
		 */
		template<typename T/*, typename = std::enable_if_t<std::is_base_of_v<Component, T>>*/>
		void remove() {
			auto component = dynamic_cast<T*>(components[typeid(T)]);
			component->destroy();
		}

		/**
		 * @tparam T Component types
		 * @return Whether the components exists for the actor
		 */
		template<typename ...T/*, typename = std::enable_if_t<std::is_base_of_v<Component, T>>*/>
		bool has() {
			return (... && (components.find(typeid(T)) != components.end()));
		}

		/**
		 * @tparam T Component type
		 * @return Component instance
		 */
		template<typename T/*, typename = std::enable_if_t<std::is_base_of_v<Component, T>>*/>
		T& get() {
			return *dynamic_cast<T*>(components[typeid(T)]);
		}

		/**
		 * @return Whether the object has been destroyed
		 */
		bool exists();

		/** Destroys this actor */
		void destroy();

		/** Clean up destroyed components */
		void clean();

		/**
		 * Creates a new actor
		 * @param name Name to give the actor
		 */
		static Actor& spawn(std::string name = "Actor");

		bool operator==(const Actor&);
		bool operator!=(const Actor&);
	private:
		static std::queue<unsigned int> recycle;
		static unsigned int nextId;

		std::unordered_map<std::type_index, Component*> components;
		bool _exists;

		Actor(unsigned int id, std::string name);
};