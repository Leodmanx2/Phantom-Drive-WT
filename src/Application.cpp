#include "Application.h"

Application::Application() {
	
}

Application::~Application() {
	
}

void Application::run() {
	std::cout << "Hello, world!" << std::endl;

	SDL_Init(SDL_INIT_EVERYTHING);

	std::cout << "WTF" << std::endl;

	SDL_Quit();
}

