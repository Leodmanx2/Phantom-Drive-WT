#include "Application.hpp"

#include "Renderer.hpp"

#include <plog/Log.h>

using namespace plog;

const unsigned int INIT_WIDTH  = 640;
const unsigned int INIT_HEIGHT = 480;

// ---------------------------------------------------------------------------
//  Constructors & Destructors
// ---------------------------------------------------------------------------

Application::Application(int argc, char** argv)
  : m_renderer(new Renderer(INIT_WIDTH, INIT_HEIGHT)) {
// Make window and OpenGl context with available extensions
#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(
	  INIT_WIDTH, INIT_HEIGHT, "Phantom Drive (WT)", nullptr, nullptr);
	if(!m_window) {
		glfwTerminate();
		throw std::runtime_error("Window or OpenGL context could not be created");
	}
	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Enable v-sync
	glfwSwapInterval(1);

	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
}

Application::~Application() { glfwDestroyWindow(m_window); }

// ---------------------------------------------------------------------------
//  Internal utility functions
// ---------------------------------------------------------------------------

void Application::key_callback(
  GLFWwindow*, int key, int, int action, int modifiers) {}

void Application::mouse_button_callback(GLFWwindow*,
                                        int button,
                                        int action,
                                        int modifiers) {}

void Application::cursor_position_callback(GLFWwindow*,
                                           double xpos,
                                           double ypos) {}

// ---------------------------------------------------------------------------
//  Public-facing functions
// ---------------------------------------------------------------------------

// Needs to be called once in order for the application to begin processing
void Application::run() {
	glfwPollEvents();
	while(!glfwWindowShouldClose(m_window)) { m_renderer->draw(); }
}
