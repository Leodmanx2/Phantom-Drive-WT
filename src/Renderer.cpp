#include "Renderer.hpp"

#include "Window.hpp"

Renderer* Renderer::s_instance = nullptr;

// There can only be one instance of Renderer at a time to keep exposure of
// tools like pick() to scripting functions simple. If you have a suggestion
// to improve on this situation, I'm all ears.
Renderer::Renderer(const std::shared_ptr<Window>& window) : m_window(window) {
	if(s_instance != nullptr)
		throw std::logic_error("Attempted to create second instance of class "
		                       "Renderer. Only one is allowed at a time.");
	init();
	s_instance = this;
}

Renderer::~Renderer() {
	clean();
	s_instance = nullptr;
}

void Renderer::init() {
	glfwGetFramebufferSize(&m_window->get(), &m_width, &m_height);

	gl::glGenFramebuffers(1, &m_frameBuffer);
	gl::glBindFramebuffer(gl::GL_FRAMEBUFFER, m_frameBuffer);

	makeRenderBuffer(
	  &m_colorAttachment, gl::GL_RGBA32F, gl::GL_COLOR_ATTACHMENT0);

	makeRenderBuffer(
	  &m_selectionAttachment, gl::GL_R32UI, gl::GL_COLOR_ATTACHMENT1);

	makeRenderBuffer(&m_depthStencilAttachment,
	                 gl::GL_DEPTH24_STENCIL8,
	                 gl::GL_DEPTH_STENCIL_ATTACHMENT);

	const gl::GLenum stat = gl::glCheckFramebufferStatus(gl::GL_FRAMEBUFFER);
	if(stat != gl::GL_FRAMEBUFFER_COMPLETE) {
		clean();
		throw std::runtime_error("Could not build framebuffer");
	}
}

void Renderer::clean() {
	gl::glDeleteFramebuffers(1, &m_frameBuffer);
	gl::glDeleteRenderbuffers(1, &m_colorAttachment);
	gl::glDeleteRenderbuffers(1, &m_selectionAttachment);
	gl::glDeleteRenderbuffers(1, &m_depthStencilAttachment);
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

void Renderer::resize() {
	int width, height;
	glfwGetFramebufferSize(&m_window->get(), &width, &height);
	if(width != m_width || height != m_height) {
		m_width  = width;
		m_height = height;
		clean();
		init();
	}
}

// ---------------------------------------------------------------------------
//  Public-facing functions
// ---------------------------------------------------------------------------

void Renderer::clear() {
	// Resize buffers if glfw-controlled default buffer size changes
	resize();

	// Clear the default framebuffer
	gl::glClearColor(0.53f, 0.88f, 0.96f, 0.0f);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT |
	            gl::GL_STENCIL_BUFFER_BIT);

	// Clear custom buffer
	gl::glBindFramebuffer(gl::GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT |
	            gl::GL_STENCIL_BUFFER_BIT);
	const int color = 0;
	gl::glClearBufferiv(gl::GL_COLOR, 1, &color);
}

void Renderer::startNormalPass() {
	const gl::GLenum drawBuffers[2] = {gl::GL_COLOR_ATTACHMENT0,
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
	gl::glBindFramebuffer(gl::GL_READ_FRAMEBUFFER, s_instance->m_frameBuffer);
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

int Renderer::width() { return s_instance->m_width; }

int Renderer::height() { return s_instance->m_height; }
