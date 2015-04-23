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
	// TODO: resize() function body
}

void Window::setTitle(const std::string* title) {
	//TODO: setTitle() function body
}

SDL_Window* Window::SDL_Pointer() const {
	return m_window;
}
