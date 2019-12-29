#ifndef PD_APPLICATION_HPP
#define PD_APPLICATION_HPP

#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <memory>

// Forward declarations ------------------------------------------------------
class Renderer;
// ---------------------------------------------------------------------------

class Application final {
	private:
	GLFWwindow*               m_window;
	std::unique_ptr<Renderer> m_renderer;

	static void error_callback(int, const char* description);
	static void
	key_callback(GLFWwindow*, int key, int, int action, int modifiers);
	static void
	mouse_button_callback(GLFWwindow*, int button, int action, int mods);
	static void
	cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

	void initFilesystem(int argc, char** argv);

	public:
	Application(int argc, char** argv);
	~Application();

	void run();
};

#endif
