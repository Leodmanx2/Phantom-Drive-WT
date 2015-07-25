#include "Application.h"

Application::Application() {
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
	catch(const std::runtime_error& exception) {
		SDL_Quit();
		g_logger->write(Logger::CRITICAL, exception.what());
		throw std::runtime_error("Could not initialize rendering system");
	}
}

Application::Application(const Application& original) {
	m_renderSystem = new RenderSystem(*original.m_renderSystem);
}

Application::~Application() {
	delete m_renderSystem;
	SDL_Quit();
}

void Application::run() {
	bool quit = false;
	SDL_Event event;
	
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				quit = true;
			}
		}
		
		m_renderSystem->draw();
	}
}

