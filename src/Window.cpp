#include "Window.h"

Window::Window(unsigned int width, unsigned int height) {
	m_window = SDL_CreateWindow("Hello World!", 
	                            SDL_WINDOWPOS_UNDEFINED, 
	                            SDL_WINDOWPOS_UNDEFINED, 
	                            width, 
	                            height, 
	                            SDL_WINDOW_OPENGL |
	                            SDL_WINDOW_SHOWN
	                            );
	if(m_window == nullptr) {
		throw std::runtime_error("SDL Window could nto be initialized");
	}
}

Window::~Window() {
	SDL_DestroyWindow(m_window);
}

void Window::resize(unsigned int width, unsigned int height) {
	SDL_SetWindowSize(m_window, width, height);
}

void Window::setTitle(const char* title) {
	SDL_SetWindowTitle(m_window, title);
}

SDL_Window* Window::SDL_Pointer() const {
	return m_window;
}
