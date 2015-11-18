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
	
	m_projectionMatrix = glm::perspective(45.0f, 
	                                      static_cast<float>(width)/static_cast<float>(height), 
	                                      0.1f, 
	                                      10000.0f);
	
	glInit();
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
	
	glInit();
}

RenderSystem::~RenderSystem() {
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
}

/** Requests that a scene draw itself onto the system's active rendering buffer
 *
 * @param [in] scene  The scene to be drawn
 */
void RenderSystem::draw(Scene* scene) {
	glClearColor( 0.53f, 0.88f, 0.96f, 0.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene->draw(m_projectionMatrix);
	
	SDL_GL_SwapWindow(m_window);
}

/**
 * Resizes the window so that the viewport matches the input parameters
 *
 * @param [in] width   The width of the viewport to size the window to
 * @param [in] height  The height of the viewport to size the widnow to
 */
void RenderSystem::resizeWindow(unsigned int width, unsigned int height) {
	m_projectionMatrix = glm::ortho(0.0f, 
	                                static_cast<float>(width), 
	                                static_cast<float>(height), 
	                                0.0f, 
	                                0.1f, 
	                                100000.0f);
																	
	SDL_SetWindowSize(m_window, width, height);
}

void RenderSystem::glInit() {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}
