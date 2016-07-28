#ifndef APPLICATION_H
#define APPLICATION_H

#include "Logger.hpp"
#include "PhysicsSimulator.hpp"
#include "RenderSystem.hpp"
#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <physfs.h>
#include <sstream>
#include <stdexcept>

class Application final {
	private:
	RenderSystem* m_renderSystem;

	Scene* m_scene; // Temporary variable for dev. purposes

	static void error_callback(int error, const char* description);

	public:
	Application(int argc, char** argv);
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	~Application();
	void run();
};

#endif
