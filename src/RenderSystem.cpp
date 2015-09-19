#include "RenderSystem.h"

RenderSystem::RenderSystem() {
	int width = 640;
	int height = 480;
	
	m_window = SDL_CreateWindow("Hello World!", 
	                            SDL_WINDOWPOS_UNDEFINED, 
	                            SDL_WINDOWPOS_UNDEFINED, 
	                            width, 
	                            height, 
	                            SDL_WINDOW_OPENGL |
	                            SDL_WINDOW_SHOWN
	                            );
	if(m_window == nullptr) {
		g_logger->write(Logger::CRITICAL, SDL_GetError());
		throw std::runtime_error("SDL Window could not be initialized");
	}
	
	m_context = SDL_GL_CreateContext(m_window);
	if(m_context == nullptr) {
		SDL_DestroyWindow(m_window);
		g_logger->write(Logger::CRITICAL, SDL_GetError());
		throw std::runtime_error("Could not create OpenGL context");
	}
	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		g_logger->write(Logger::CRITICAL, (const char*)glewGetErrorString(err));
		throw std::runtime_error("Failed to initialize GLEW");
	}
	
	m_projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
}

RenderSystem::RenderSystem(const RenderSystem& original) {
	int width, height;
	SDL_GetWindowSize(original.m_window, &width, &height);
	
	const char* title = SDL_GetWindowTitle(original.m_window);
	
	Uint32 flags = SDL_GetWindowFlags(original.m_window);
	
	m_window = SDL_CreateWindow(title, 
	                            SDL_WINDOWPOS_UNDEFINED, 
	                            SDL_WINDOWPOS_UNDEFINED, 
	                            width, 
	                            height, 
	                            flags
	                            );
	if(m_window == nullptr) {
		throw std::runtime_error("SDL Window could not be initialized");
	} 
}

RenderSystem::~RenderSystem() {
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

void RenderSystem::draw(Scene* scene) {
	glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float* projectionData = glm::value_ptr(m_projectionMatrix);
	
	scene->draw(projectionData);
	
	SDL_GL_SwapWindow(m_window);
}

void RenderSystem::resizeWindow(unsigned int width, unsigned int height) {
	m_projectionMatrix = glm::ortho(0.0f, (float)width, (float)height, 0.0f, 0.1f, 100.0f);
	SDL_SetWindowSize(m_window, width, height);
}
