#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include <iostream>
#include "Window.h"

class RenderSystem {
	private:
		SDL_Renderer* m_renderer;
		Window* m_window;
	
	public:
		RenderSystem();
		~RenderSystem();

		void draw();
};

#endif

