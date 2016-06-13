#include "Application.h"

Application::Application(int argc, char** argv) {
	g_logger->write(Logger::INFO, "Initializing virtual file system");
	if(PHYSFS_init(argv[0]) == 0) {
		g_logger->write(Logger::CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not initialize virtual file system");
	}
	
	std::string assetDir = PHYSFS_getBaseDir() + std::string("ass");
	std::string logMessage = "Mounting asset location: " + assetDir;
	g_logger->write(Logger::INFO, logMessage.c_str());
	if(PHYSFS_mount(assetDir.c_str(), "/", 1) == 0) {
		g_logger->write(Logger::CRITICAL, PHYSFS_getLastError());
		throw std::runtime_error("Could not mount asset files");
	}
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		g_logger->write(Logger::CRITICAL, SDL_GetError());
		throw std::runtime_error("Failed to initialize SDL_video");
	}

	try { m_renderSystem = new RenderSystem(); }
	catch(const std::exception& exception) {
		SDL_Quit();
		g_logger->write(Logger::CRITICAL, exception.what());
		throw std::runtime_error("Could not initialize rendering system");
	}
	
	m_scene = new Scene();
}

Application::Application(const Application& original) {
	m_renderSystem = new RenderSystem(*original.m_renderSystem);
	m_scene = new Scene(*original.m_scene);
}

Application::~Application() {
	delete m_renderSystem;
	delete m_scene;
	SDL_Quit();
	PHYSFS_deinit();
}

/**
 * Needs to be called once in order for the application to begin processing
 */
void Application::run() {
	bool quit = false;
	SDL_Event event;
	
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = true;
			}
		}
		m_scene->simulate();
		m_scene->update();
		m_renderSystem->draw(m_scene);
	}
}

