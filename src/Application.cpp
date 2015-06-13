#include "Application.h"

Application::Application() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
	}

	try { m_renderSystem = new RenderSystem(); }
	catch(int e) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
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

