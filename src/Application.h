#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <physfs.h>
#include "RenderSystem.h"
#include "Logger.h"

class Application {
	private:
		RenderSystem* m_renderSystem;
	
	public:
		Application();
		Application(const Application& original);
		~Application();
		void run();
};

#endif

