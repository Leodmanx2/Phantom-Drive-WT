#include "RenderSystem.h"

RenderSystem::RenderSystem() {
	try { m_window = new Window(640, 480); }
	catch(int e) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		throw -1;
	}
	
	m_renderer = SDL_CreateRenderer(m_window->SDL_Pointer(), 
	                                -1, 
	                                SDL_RENDERER_ACCELERATED | 
	                                SDL_RENDERER_PRESENTVSYNC
	                                );

	if(m_renderer == nullptr) {
		delete m_window;
		throw -1;
	}
}

RenderSystem::~RenderSystem() {
	SDL_DestroyRenderer(m_renderer);
	delete m_window;
}

void RenderSystem::draw() {
	SDL_SetRenderDrawColor(m_renderer, 135, 225, 245, 255);
	SDL_RenderClear(m_renderer);
	SDL_RenderPresent(m_renderer);
}
