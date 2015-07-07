#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <SDL2/SDL.h>
#include <stdexcept>
#include "RenderSystem.h"
#include "Logger.h"
#include <sstream>

class Application {
	private:
		RenderSystem* m_renderSystem;
	
	public:
		Application();
		~Application();
		void run();
};

#endif

