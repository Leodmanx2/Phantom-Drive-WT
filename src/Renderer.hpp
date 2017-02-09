#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.hpp"
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer final {
	private:
	GLFWwindow* m_window;

	int m_width;
	int m_height;

	gl::GLuint m_frameBuffer;
	gl::GLuint m_colorAttachment;
	gl::GLuint m_selectionAttachment;
	gl::GLuint m_depthStencilAttachment;

	glm::mat4 m_projectionMatrix;

	void
	makeRenderBuffer(gl::GLuint* bufferID, gl::GLenum format, gl::GLenum target);

	void init(int width, int height);
	void clean();

	public:
	Renderer(GLFWwindow* window);
	~Renderer();

	void draw(Scene& scene);
};

#endif
