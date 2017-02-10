#ifndef RENDERER_H
#define RENDERER_H

#include "Scene.hpp"
#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Renderer encapsulates non-core rendering-related functionality. In other
// words, it contains everything that applies to the graphics system as a
// whole that isn't provided directly by GLFW.
//
// I know the boundaries are kind of vague, but tring to modularize code that
// relies on a global state machine is sort of difficult. If you have
// suggestions, I'd love to hear them.
class Renderer final {
	private:
	GLFWwindow* m_window;

	int m_width;
	int m_height;

	gl::GLuint m_frameBuffer;
	gl::GLuint m_colorAttachment;
	gl::GLuint m_selectionAttachment;
	gl::GLuint m_depthStencilAttachment;

	void
	makeRenderBuffer(gl::GLuint* bufferID, gl::GLenum format, gl::GLenum target);

	void init(int width, int height);
	void clean();

	public:
	~Renderer();

	void setWindow(GLFWwindow* window);

	// Render-related tools
	int pick(int frameCoordX, int frameCoordY);

	// Rendering phase controls
	void clear();
	void startNormalPass();
	void finishNormalPass();

	// Member acessors
	int width() const;
	int height() const;
};

extern Renderer g_renderer;

#endif
