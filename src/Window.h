#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>
#include <stdexcept>

class Window {
	private:
		SDL_Window* m_window;

	public:
		Window(unsigned int width, unsigned int height);
		~Window();

		void resize(unsigned int width, unsigned int height);
		void setTitle(const char* title);
		
		SDL_Window* SDL_Pointer() const;
};

#endif

