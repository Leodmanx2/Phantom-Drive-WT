#ifndef APPLICATION_H
#define APPLICATION_H

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include "Logger.hpp"
#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <physfs.h>
#include <sstream>
#include <stdexcept>

class Application final {
	private:
	GLFWwindow* m_window;
	glm::mat4   m_projectionMatrix;

	static const int default_width  = 640;
	static const int default_height = 480;

	void initFilesystem(int argc, char** argv);
	void initGraphics();
	void initIO();

	void draw(Scene& scene);
	void resizeWindow(unsigned int width, unsigned int height);
	void processInput();

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
