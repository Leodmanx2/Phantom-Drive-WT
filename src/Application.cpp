#include "Application.h"

Application::Application() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		// TODO: Log
		std::cerr << "Could not initilize SDL: " << SDL_GetError() << std::endl;
	}

	try { m_renderSystem = new RenderSystem(); }
	catch(const std::exception exception) {
		SDL_Quit();
		// TODO: Log
		std::cerr << "RenderSystem: " << exception.what() << std::endl;
		throw std::runtime_error(std::string("Failed to initialize application (") + exception.what() + ")");
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

