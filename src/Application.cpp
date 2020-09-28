#include "Application.hpp"

#include "Renderer.hpp"
#include <physfs.h>
#include <plog/Log.h>

using namespace plog;

const unsigned int INIT_WIDTH  = 640;
const unsigned int INIT_HEIGHT = 640;

// ---------------------------------------------------------------------------
//  Constructors & Destructors
// ---------------------------------------------------------------------------

Application::Application(int argc, char** argv)
  : m_renderer(new Renderer(INIT_WIDTH, INIT_HEIGHT)) {
	initFilesystem(argc, argv);

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

Application::~Application() {
	PHYSFS_deinit();
	glfwDestroyWindow(m_window);
}

// ---------------------------------------------------------------------------
//  Subsystem initialization helper functions
// ---------------------------------------------------------------------------

void Application::initFilesystem(int, char** argv) {
	LOG(info) << "Initializing virtual file system";
	if(PHYSFS_init(argv[0]) == 0) {
		LOG(fatal) << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		throw std::runtime_error("could not initialize virtual file system");
	}

	const std::string assetDir = PHYSFS_getBaseDir() + std::string("ass");
	LOG(info) << "mounting asset location: " << assetDir;
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		PHYSFS_deinit();
		LOG(fatal) << PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		throw std::runtime_error("could not mount asset location");
	}
}

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
