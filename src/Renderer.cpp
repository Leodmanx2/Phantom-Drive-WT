#include "Renderer.hpp"

#include "Geometry.hpp"
#include "pmdl.hpp"
#include "utility.hpp"
#include <algorithm>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <globjects/Renderbuffer.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>

using namespace globjects;
using namespace gl;
using namespace std;

Renderer::Renderer(unsigned int width, unsigned int height)
  : m_width(width), m_height(height) {
	// Enable back-face culling, z-buffering, and anti-aliasing
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);

	init();
}

void Renderer::init() {
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

	const GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(stat != GL_FRAMEBUFFER_COMPLETE) {
		throw runtime_error("Could not build framebuffer");
	}
}

void Renderer::resize(int width, int height) {
	if(width != m_width || height != m_height) {
		m_width  = width;
		m_height = height;
		init();
	}
}

void Renderer::clear() {
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

		shared_ptr<ShaderProgram> shader = m_shaderCache.get(task.keys.shader);
		shader->use();

		shader->setUniform("id", task.id);
		shader->setUniform("model", task.model);
		shader->setUniform("view", task.view);
		shader->setUniform("projection", task.projection);
		shader->setUniform("eyePos", task.eye);
		shader->setUniform("ambience", task.ambience);
		shader->setUniform("normal", inverseTranspose(task.model * task.view));

		m_textureCache.get(task.keys.diffuse)->bindActive(0);
		m_textureCache.get(task.keys.specular)->bindActive(1);

		shared_ptr<Geometry> geometry = m_geometryCache.get(task.keys.geometry);
		const VertexArray&   vao      = geometry->vao();
		int                  elements = geometry->elements();
		vao.bind();
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_CONSTANT_COLOR, GL_ONE);
		const int lightFactor = 1.0 / task.lights.size();
		glBlendColor(lightFactor, lightFactor, lightFactor, lightFactor);
		for(const Light& light : task.lights) {
			// TODO: Update test shaders to match new model
			shader->setUniform("light.position", light.position);
			shader->setUniform("light.direction", light.direction);
			shader->setUniform("light.color", light.color);
			shader->setUniform("light.intensity", light.intensity);
			shader->setUniform("light.angle", light.angle);
			shader->setUniform("light.radius", light.radius);
			vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
		}
		glDisable(GL_BLEND);
		vao.unbind();

		shader->release();

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
}

void Renderer::queue(RenderTask task) {
	if(!m_textureCache.get(task.keys.diffuse)) {
		shared_ptr<Texture> diffuse(loadTexture(task.keys.diffuse));
		m_textureCache.put(task.keys.diffuse, diffuse);
	}

	if(!m_textureCache.get(task.keys.specular)) {
		shared_ptr<Texture> specular(loadTexture(task.keys.specular));
		m_textureCache.put(task.keys.specular, specular);
	}

	if(!m_geometryCache.get(task.keys.geometry)) {
		shared_ptr<Geometry> geometry = make_shared<Geometry>(task.keys.geometry);
		m_geometryCache.put(task.keys.geometry, geometry);
	}

	if(!m_shaderCache.get(task.keys.shader)) {
		shared_ptr<ShaderProgram> shader =
		  make_shared<ShaderProgram>(task.keys.shader);
		m_shaderCache.put(task.keys.shader, shader);
	}

	m_queue.push(task);
}

unique_ptr<Texture> loadTexture(const string& name) {
	std::string buffer = readFile(name);

	// Create a GLI texture from the data read in by PhysFS
	gli::texture          texture = gli::load(buffer.data(), buffer.size());
	gli::gl               gl(gli::gl::PROFILE_GL33);
	const gli::gl::format format =
	  gl.translate(texture.format(), texture.swizzles());
	GLenum target = static_cast<GLenum>(gl.translate(texture.target()));
	if(target != GL_TEXTURE_2D) {
		throw std::runtime_error(
		  "Texture target is not GL_TEXTURE_2D/gli::TARGET_2D");
	}
	if(texture.empty()) {
		throw std::runtime_error(name + std::string(" is not a valid texture"));
	}

	// Reserve memory on the GPU for texture and describe its layout
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(target, textureID);

	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, texture.levels() - 1);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzles[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzles[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzles[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzles[3]);

	const gli::tvec3<GLsizei> extent(texture.extent());

	glTexStorage2D(target,
	               texture.levels(),
	               static_cast<GLenum>(format.Internal),
	               extent.x,
	               extent.y);

	// Write image data to GPU memory
	for(std::size_t layer = 0; layer < texture.layers(); ++layer) {
		for(std::size_t face = 0; face < texture.faces(); ++face) {
			for(std::size_t level = 0; level < texture.levels(); ++level) {
				if(gli::is_compressed(texture.format())) {
					glCompressedTexSubImage2D(target,
					                          level,
					                          0,
					                          0,
					                          extent.x,
					                          extent.y,
					                          static_cast<GLenum>(format.Internal),
					                          texture.size(level),
					                          texture.data(layer, face, level));
				} else {
					glTexSubImage2D(target,
					                level,
					                0,
					                0,
					                extent.x,
					                extent.y,
					                static_cast<GLenum>(format.External),
					                static_cast<GLenum>(format.Type),
					                texture.data(layer, face, level));
				}
			}
		}
	}

	return Texture::fromId(textureID, GL_TEXTURE_2D);
}
