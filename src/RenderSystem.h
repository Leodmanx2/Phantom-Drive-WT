#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "Window.h"

class RenderSystem {
	private:
		SDL_GLContext m_context;
		Window* m_window;
	
	public:
		RenderSystem();
		~RenderSystem();

		void draw();
};

#endif

