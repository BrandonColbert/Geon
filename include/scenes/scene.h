#pragma once

class Scene {
	public:
		void start();
	protected:
		virtual void create() = 0;
};