#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <SDL2/SDL.h>
#include "RenderSystem.h"

class Application {
	private:
		RenderSystem* m_rendererSystem;
	
	public:
		Application();
		~Application();
		void run();
};

#endif

