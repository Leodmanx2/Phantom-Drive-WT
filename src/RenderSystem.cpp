#include "RenderSystem.h"

RenderSystem::RenderSystem() {
	try { m_window = new Window(640, 480); }
	catch(const std::exception exception) {
		throw std::runtime_error(std::string("Failed to create window: ") + exception.what());
	}
	
	m_context = SDL_GL_CreateContext(m_window->SDL_Pointer());
	if(m_context == nullptr) {
		delete m_window;
		throw std::runtime_error(SDL_GetError());
	}
	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		 throw std::runtime_error((const char*)glewGetErrorString(err));
	}
}

RenderSystem::~RenderSystem() {
	SDL_GL_DeleteContext(m_context);
	delete m_window;
}

void RenderSystem::draw() {
	glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	SDL_GL_SwapWindow(m_window->SDL_Pointer());
}
