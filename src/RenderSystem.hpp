#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#define GLEW_STATIC

#include "Scene.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iostream>
#include <stdexcept>

class RenderSystem {
	private:
	SDL_GLContext m_context;
	SDL_Window*   m_window;

	glm::mat4 m_projectionMatrix;

	void glInit();

	public:
	RenderSystem();
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	~RenderSystem();

	void draw(Scene* scene);
	void resizeWindow(unsigned int width, unsigned int height);
};

#endif
