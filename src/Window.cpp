#include "Window.h"

Window::Window(unsigned int width, unsigned int height) {
	m_window = SDL_CreateWindow("Hello World!", 
	                            100, 
	                            100, 
	                            width, 
	                            height, 
	                            SDL_WINDOW_SHOWN
	                            );
	if(m_window == nullptr) {
		throw -1;
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
