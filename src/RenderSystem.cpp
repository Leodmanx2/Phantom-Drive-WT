#include "RenderSystem.hpp"

#define LOG_GL
#include "glerr.hpp"

RenderSystem::RenderSystem() {
	int width  = 640;
	int height = 480;

	m_window = glfwCreateWindow(width, height, "Hello World!", nullptr, nullptr);
	if(!m_window) {
		throw std::runtime_error("Window or OpenGL context could not be created");
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glbinding::Binding::initialize();

	m_projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(width) / static_cast<float>(height),
	                   0.1f,
	                   10000.0f);

	glInit();
}

RenderSystem::~RenderSystem() { glfwDestroyWindow(m_window); }

/** Requests that a scene draw itself onto the system's active rendering buffer
 *
 * @param [in] scene  The scene to be drawn
 */
void RenderSystem::draw(Scene& scene) {
	//glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	gl::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
	glLogErr("Clearing buffers");

	scene.draw(m_projectionMatrix);
	glLogErr("Drawing scene");

	glfwSwapBuffers(m_window);
	glLogErr("Swapping buffers");
}

/**
 * Resizes the window so that the viewport matches the input parameters
 *
 * @param [in] width   The width of the viewport to size the window to
 * @param [in] height  The height of the viewport to size the widnow to
 */
void RenderSystem::resizeWindow(unsigned int width, unsigned int height) {
	m_projectionMatrix = glm::ortho(0.0f,
	                                static_cast<float>(width),
	                                static_cast<float>(height),
	                                0.0f,
	                                0.1f,
	                                100000.0f);

	glfwSetWindowSize(m_window, width, height);
}

void RenderSystem::glInit() {
	gl::glEnable(gl::GL_CULL_FACE);
	gl::glEnable(gl::GL_DEPTH_TEST);
	gl::glEnable(gl::GL_LINE_SMOOTH);
}

bool RenderSystem::running() { return !glfwWindowShouldClose(m_window); }
