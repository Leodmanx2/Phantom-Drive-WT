#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>
#include "Window.h"

#include "Actor.h"

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

