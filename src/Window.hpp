#ifndef WINDOW_H
#define WINDOW_H

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include "Logger.hpp"
#include <GLFW/glfw3.h>
#include <sstream>

class Window final {
	private:
	static const int default_width  = 640;
	static const int default_height = 480;

	GLFWwindow* m_window;

	static void error_callback(int, const char* description);

	public:
	Window();
	Window(const Window&) = delete;
	~Window();

	void initGL();

	GLFWwindow& get() const;
};

#endif
