#include "Window.hpp"

Window::Window() {
// Make window and OpenGl context with available extensions
#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(
	  default_width, default_height, "Hello World!", nullptr, nullptr);
	if(!m_window) {
		glfwTerminate();
		throw std::runtime_error("Window or OpenGL context could not be created");
	}
	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	initGL();

	// Enable v-sync
	glfwSwapInterval(1);
}

Window::~Window() { glfwDestroyWindow(m_window); }

void Window::initGL() {
	glbinding::Binding::initialize();

// Enable automatic logging of OpenGL calls in debug mode
#ifdef DEBUG
	glbinding::setCallbackMaskExcept(glbinding::CallbackMask::After,
	                                 {"glGetError"});
	glbinding::setAfterCallback([](const glbinding::FunctionCall& call) {
		const gl::GLenum error = gl::glGetError();
		if(error != gl::GL_NO_ERROR) {
			std::stringstream ss("OpenGL Error: ");
			ss << std::hex << error << " after function: " << call.function->name();
			g_logger.write(Logger::LogLevel::LOG_DEBUG, ss.str());
		}
	});
#endif

	// Log loaded OpenGl version
	std::stringstream version;
	version << "OpenGl context version: " << gl::glGetString(gl::GL_VERSION);
	g_logger.write(Logger::LogLevel::LOG_INFO, version.str());

	// Enable back-face culling, z-buffering, and anti-aliasing
	gl::glEnable(gl::GL_CULL_FACE);
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glEnable(gl::GL_LINE_SMOOTH);
}

GLFWwindow& Window::get() const { return *m_window; }
