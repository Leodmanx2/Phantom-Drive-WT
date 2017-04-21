#ifndef APPLICATION_H
#define APPLICATION_H

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include "EditorScene.hpp"
#include "Logger.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <SWI-cpp.h>
#include <functional>
#include <iostream>
#include <physfs.h>
#include <sstream>
#include <stdexcept>

class Application final {
	private:
	std::shared_ptr<Window> m_window;
	Renderer                m_renderer;

	void initFilesystem(int argc, char** argv);

	void processInput();
	void draw();

	Scene* m_scene; // Temporary variable for dev. purposes

	public:
	Application(int argc, char** argv);
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	~Application();

	void run();
};

#endif
