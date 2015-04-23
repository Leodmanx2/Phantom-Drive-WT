#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <SDL2/SDL.h>

class Application {
	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
	
	public:
		Application();
		~Application();
		void run();
};

#endif

