#include "Renderer.hpp"

#include <glbinding/gl/gl.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <globjects/Renderbuffer.h>
#include <globjects/Texture.h>

using namespace globjects;
using namespace gl;
using namespace std;

void Renderer::init() {
	glfwGetFramebufferSize(&m_window->get(), &m_width, &m_height);

	m_colorAttachment = make_unique<Renderbuffer>();
	m_colorAttachment->storage(GL_RGBA32F, m_width, m_height);

	m_selectionAttachment = make_unique<Renderbuffer>();
	m_selectionAttachment->storage(GL_R32UI, m_width, m_height);

	m_depthStencilAttachment = make_unique<Renderbuffer>();
	m_depthStencilAttachment->storage(GL_DEPTH24_STENCIL8, m_width, m_height);

	m_frameBuffer = make_unique<Framebuffer>();
	m_frameBuffer->attachRenderBuffer(GL_COLOR_ATTACHMENT0,
	                                  m_colorAttachment.get());
	m_frameBuffer->attachRenderBuffer(GL_COLOR_ATTACHMENT1,
	                                  m_selectionAttachment.get());
	m_frameBuffer->attachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT,
	                                  m_depthStencilAttachment.get());
	m_frameBuffer->setDrawBuffers({GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});

	const GLenum stat = glCheckFramebufferStatus(gl::GL_FRAMEBUFFER);
	if(stat != GL_FRAMEBUFFER_COMPLETE) {
		throw runtime_error("Could not build framebuffer");
	}
}

void Renderer::resize() {
	int width, height;
	glfwGetFramebufferSize(&m_window->get(), &width, &height);
	if(width != m_width || height != m_height) {
		m_width  = width;
		m_height = height;
		init();
	}
}

void Renderer::clear() {
	// Resize buffers if glfw-controlled default buffer size changes
	resize();

	// Clear the default framebuffer
	glClearColor(0.53f, 0.88f, 0.96f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Clear custom buffer
	m_frameBuffer->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
	                     GL_STENCIL_BUFFER_BIT);
	const int color = 0;
	m_frameBuffer->clearBuffer(GL_COLOR, 1, &color);
}

void Renderer::draw() {
	clear();

	while(!m_queue.empty()) {
		RenderTask task = m_queue.front();

		auto shader = m_shaderCache.get(task.keys.shader);

		shader->setUniform("id", task.id);
		shader->setUniform("model", task.model);
		shader->setUniform("view", task.view);
		shader->setUniform("projection", task.projection);
		shader->setUniform("eyePos", task.eye);
		shader->setUniform("ambience", task.ambience);
		shader->setUniform("normal", inverseTranspose(task.model * task.view));

		m_textureCache.get(task.keys.diffuse)->bindActive(0);
		m_textureCache.get(task.keys.specular)->bindActive(1);

		auto vao = m_geometryCache.get(task.keys.geometry);
		vao->bind();
		vao->drawElements(GL_TRIANGLES, task.keys.elementCount, GL_UNSIGNED_INT);
		vao->unbind();

		m_queue.pop();
	}

	m_frameBuffer->bind(GL_READ_FRAMEBUFFER);
	m_frameBuffer->unbind(GL_DRAW_FRAMEBUFFER);
	glBlitFramebuffer(0,
	                  0,
	                  m_width,
	                  m_height,
	                  0,
	                  0,
	                  m_width,
	                  m_height,
	                  GL_COLOR_BUFFER_BIT,
	                  GL_NEAREST);

	glfwSwapBuffers(&m_window->get());
}
