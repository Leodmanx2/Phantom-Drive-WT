#include "Application.hpp"

Application::Application(int argc, char** argv) {
	g_logger->write(Logger::INFO, "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger->write(Logger::CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not initialize virtual file system");
	}

	std::string assetDir   = PHYSFS_getBaseDir() + std::string("ass");
	std::string logMessage = "Mounting asset location: " + assetDir;
	g_logger->write(Logger::INFO, logMessage.c_str());
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		g_logger->write(Logger::CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not mount asset files");
	}

	glfwSetErrorCallback(error_callback);
	if(!glfwInit()) { throw std::runtime_error("GLFW initialization failed"); }

	try {
		m_renderSystem = new RenderSystem();
	} catch(const std::exception& exception) {
		glfwTerminate();
		g_logger->write(Logger::CRITICAL, exception.what());
		throw std::runtime_error("Could not initialize rendering system");
	}

	m_scene = new Scene();
}

Application::~Application() {
	delete m_renderSystem;
	delete m_scene;
	glfwTerminate();
	PHYSFS_deinit();
}

void Application::error_callback(int error, const char* description) {
	g_logger->write(Logger::ERROR, description);
}

/**
 * Needs to be called once in order for the application to begin processing
 */
void Application::run() {
	while(m_renderSystem->running()) {
		m_scene->simulate();
		m_scene->update();
		m_renderSystem->draw(m_scene);
		glfwPollEvents();
	}
}
