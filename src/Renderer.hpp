#ifndef RENDERER_H
#define RENDERER_H

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#define GLFW_INCLUDE_NONE

#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

class Renderer final {
	private:
	static Renderer* s_instance;

	std::shared_ptr<Window> m_window;

	int m_width;
	int m_height;

	gl::GLuint m_frameBuffer;
	gl::GLuint m_colorAttachment;
	gl::GLuint m_selectionAttachment;
	gl::GLuint m_depthStencilAttachment;

	void
	makeRenderBuffer(gl::GLuint* bufferID, gl::GLenum format, gl::GLenum target);
	void init();
	void clean();
	void resize();

	public:
	explicit Renderer(const std::shared_ptr<Window>& window);
	Renderer(const Renderer&) = delete;
	~Renderer();

	// Render-related tools
	static int pick(int frameCoordX, int frameCoordY);

	// Rendering phase controls
	void clear();
	void startNormalPass();
	void finishNormalPass();

	// Member accessors
	static int width();
	static int height();
};

#endif
