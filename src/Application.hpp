#ifndef APPLICATION_H
#define APPLICATION_H

#include "Logger.hpp"
#include "PhysicsSimulator.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <physfs.h>
#include <sstream>
#include <stdexcept>

class Application {
	private:
	RenderSystem* m_renderSystem;

	Scene* m_scene; // Temporary variable for dev. purposes

	public:
	Application(int argc, char** argv);
	Application(const Application& original);
	~Application();
	void run();
};

#endif
