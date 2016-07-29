#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include "Scene.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iostream>
#include <stdexcept>

class RenderSystem final {
	private:
	GLFWwindow* m_window;

	glm::mat4 m_projectionMatrix;

	void glInit();

	public:
	RenderSystem();
	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	~RenderSystem();

	bool running();

	void draw(Scene& scene);
	void resizeWindow(unsigned int width, unsigned int height);
};

#endif
