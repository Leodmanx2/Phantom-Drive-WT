#include "RenderSystem.h"

RenderSystem::RenderSystem() {
	try { m_window = new Window(640, 480); }
	catch(int e) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		throw -1;
	}
	
	m_context = SDL_GL_CreateContext(m_window->SDL_Pointer());

	if(m_context == nullptr) {
		delete m_window;
		throw -1;
	}
	
	glewInit();
	
	m_actor = new Actor();
}

RenderSystem::~RenderSystem() {
	SDL_GL_DeleteContext(m_context);
	delete m_window;
	
	delete m_actor;
}

void RenderSystem::draw() {
	glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_actor->draw();
	
	SDL_GL_SwapWindow(m_window->SDL_Pointer());
}
