#include "Application.hpp"

// ---------------------------------------------------------------------------
//  Constructors & Destructors
// ---------------------------------------------------------------------------

Application::Application(int argc, char** argv)
  : m_window(new Window()), m_renderer(m_window) {
	initFilesystem(argc, argv);

	m_scene = new EditorScene("DevScene");
}

Application::~Application() {
	delete m_scene;
	PHYSFS_deinit();
}

// ---------------------------------------------------------------------------
//  Subsystem initialization helper functions
// ---------------------------------------------------------------------------

void Application::initFilesystem(int, char** argv) {
	g_logger.write(Logger::LOG_INFO, "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger.write(Logger::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not initialize virtual file system");
	}

	const std::string assetDir   = PHYSFS_getBaseDir() + std::string("ass");
	const std::string logMessage = "Mounting asset location: " + assetDir;
	g_logger.write(Logger::LOG_INFO, logMessage.c_str());
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		PHYSFS_deinit();
		g_logger.write(Logger::LOG_CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not mount asset location");
	}
}

// ---------------------------------------------------------------------------
//  Internal utility functions
// ---------------------------------------------------------------------------

void Application::processInput() {
	glfwPollEvents();
	GLFWwindow& window = m_window->get();
	if(glfwGetKey(&window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(&window, GLFW_TRUE);

	m_scene->processInput(window);
}

void Application::draw() {
	m_renderer.clear();
	m_renderer.startNormalPass();
	m_scene->draw();
	m_renderer.finishNormalPass();
	glfwSwapBuffers(&m_window->get());
}

// ---------------------------------------------------------------------------
//  Public-facing functions
// ---------------------------------------------------------------------------

/**
 * Needs to be called once in order for the application to begin processing
 */
void Application::run() {
	std::chrono::time_point<std::chrono::steady_clock> oldTime, newTime;
	oldTime = newTime = std::chrono::steady_clock::now();
	while(!glfwWindowShouldClose(&m_window->get())) {
		std::chrono::milliseconds timeStep =
		  std::chrono::duration_cast<std::chrono::milliseconds>(newTime - oldTime);
		m_scene->update(timeStep);
		draw();
		processInput();
		oldTime = newTime;
		newTime = std::chrono::steady_clock::now();
	}
}
