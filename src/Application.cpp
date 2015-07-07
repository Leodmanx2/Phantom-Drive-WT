#include "Application.h"

Application::Application() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
			
			m_renderSystem->draw();
		}
	}
}

