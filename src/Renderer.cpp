#include "Renderer.hpp"

Renderer g_renderer{};

void Renderer::setWindow(GLFWwindow* window) {
	m_window = window;
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	init(width, height);
}

void Renderer::init(int width, int height) {
	m_width  = width;
	m_height = height;

	gl::glGenFramebuffers(1, &m_frameBuffer);
	gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, m_frameBuffer);

	makeRenderBuffer(
	  &m_colorAttachment, gl::GL_RGBA32F, gl::GL_COLOR_ATTACHMENT0);

	makeRenderBuffer(
	  &m_selectionAttachment, gl::GL_R32UI, gl::GL_COLOR_ATTACHMENT1);

	makeRenderBuffer(&m_depthStencilAttachment,
	                 gl::GL_DEPTH24_STENCIL8,
	                 gl::GL_DEPTH_STENCIL_ATTACHMENT);

	gl::GLenum stat = gl::glCheckFramebufferStatus(gl::GL_FRAMEBUFFER);
	if(stat != gl::GL_FRAMEBUFFER_COMPLETE) {
		clean();
		throw std::runtime_error("Could not build framebuffer");
	}
}

void Renderer::makeRenderBuffer(gl::GLuint* bufferID,
                                gl::GLenum  format,
                                gl::GLenum  target) {
	gl::glGenRenderbuffers(1, bufferID);
	gl::glBindRenderbuffer(gl::GL_RENDERBUFFER, *bufferID);
	gl::glRenderbufferStorage(gl::GL_RENDERBUFFER, format, m_width, m_height);
	gl::glFramebufferRenderbuffer(
	  gl::GL_FRAMEBUFFER, target, gl::GL_RENDERBUFFER, *bufferID);
}

Renderer::~Renderer() { clean(); }

void Renderer::clean() {
	gl::glDeleteFramebuffers(1, &m_frameBuffer);
	gl::glDeleteRenderbuffers(1, &m_colorAttachment);
	gl::glDeleteRenderbuffers(1, &m_selectionAttachment);
	gl::glDeleteRenderbuffers(1, &m_depthStencilAttachment);
}

void Renderer::clear() {
	// Resize buffers if glfw-controlled default buffer size changes
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	if(width != m_width || height != m_height) {
		clean();
		init(width, height);
	}

	// Clear the default framebuffer
	gl::glClearColor(0.53f, 0.88f, 0.96f, 0.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT |
	            gl::GL_STENCIL_BUFFER_BIT);

	// Clear custom buffer
	gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT |
	            gl::GL_STENCIL_BUFFER_BIT);
	int color = 0;
	gl::glClearBufferiv(gl::GL_COLOR, 1, &color);
}

void Renderer::startNormalPass() {
	gl::GLenum drawBuffers[2] = {gl::GL_COLOR_ATTACHMENT0,
	                             gl::GL_COLOR_ATTACHMENT1};
	gl::glDrawBuffers(2, drawBuffers);
}

void Renderer::finishNormalPass() {
	// Copy framebuffer output to default framebuffer
	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, m_frameBuffer);
	gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, 0);
	gl::glBlitFramebuffer(0,
	                      0,
	                      m_width,
	                      m_height,
	                      0,
	                      0,
	                      m_width,
	                      m_height,
	                      gl::GL_COLOR_BUFFER_BIT,
	                      gl::GL_NEAREST);

	// Clean up
	// The draw buffer has already been reset.
	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);
}

int Renderer::pick(int frameCoordX, int frameCoordY) {
	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, m_frameBuffer);
	gl::glReadBuffer(gl::GL_COLOR_ATTACHMENT1);
	std::uint32_t selectedID;
	gl::glReadPixels(frameCoordX,
	                 frameCoordY,
	                 1,
	                 1,
	                 gl::GL_RED_INTEGER,
	                 gl::GL_UNSIGNED_INT,
	                 &selectedID);
	gl::glReadBuffer(gl::GL_COLOR_ATTACHMENT0);
	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, 0);
	return selectedID;
}

int Renderer::width() const { return m_width; }

int Renderer::height() const { return m_height; }
