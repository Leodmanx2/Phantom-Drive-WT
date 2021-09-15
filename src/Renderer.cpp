#include "Renderer.hpp"

#include "Geometry.hpp"
#include "PMDL.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <globjects/ProgramPipeline.h>
#include <globjects/Renderbuffer.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <plog/Log.h>

using namespace globjects;
using namespace gl;
using namespace std;

RenderTask::RenderTask(const RenderComponent&    component,
                       int                       id,
                       glm::mat4                 model,
                       glm::mat4                 view,
                       glm::mat4                 projection,
                       glm::vec3                 eye,
                       float                     ambience,
                       const std::vector<Light>& lights)
  : keys(component)
  , id(id)
  , model(model)
  , view(view)
  , projection(projection)
  , eye(eye)
  , ambience(ambience)
  , lights(lights) {}

Renderer::Renderer(unsigned int width, unsigned int height)
  : m_width(width), m_height(height) {
	// Enable back-face culling, z-buffering, and anti-aliasing
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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
		throw runtime_error("could not build framebuffer");
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
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Clear custom buffer
	m_frameBuffer->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
	                     GL_STENCIL_BUFFER_BIT);
	const int color = 0;
	m_frameBuffer->clearBuffer(GL_COLOR, 1, &color);
}

void Renderer::draw() {
	clear();

	m_frameBuffer->bind(GL_DRAW_FRAMEBUFFER);

	while(!m_queue.empty()) {
		RenderTask task = m_queue.front();

		shared_ptr<ShaderProgram> vertexShader =
		  m_shaderCache.get(task.keys.vertexShader);
		shared_ptr<ShaderProgram> fragmentShader =
		  m_shaderCache.get(task.keys.fragmentShader);

		ProgramPipeline pipeline;
		pipeline.useStages(&vertexShader->get(), gl::GL_VERTEX_SHADER_BIT);
		pipeline.useStages(&fragmentShader->get(), gl::GL_FRAGMENT_SHADER_BIT);
		pipeline.use();

		vertexShader->setUniform("model", task.model);
		vertexShader->setUniform("view", task.view);
		vertexShader->setUniform("projection", task.projection);
		vertexShader->setUniform("normalMatrix",
		                         inverseTranspose(task.model * task.view));

		fragmentShader->setUniform("view", task.view);
		fragmentShader->setUniform("id", task.id);
		fragmentShader->setUniform("eyePos", task.eye);
		fragmentShader->setUniform("ambience", task.ambience);

		m_textureCache.get(task.keys.diffuse)->bindActive(0);
		m_textureCache.get(task.keys.specular)->bindActive(1);

		shared_ptr<Geometry> geometry = m_geometryCache.get(task.keys.geometry);
		const VertexArray&   vao      = geometry->vao();
		int                  elements = geometry->elements();
		vao.bind();

		fragmentShader->setUniform("useAmbient", true);
		fragmentShader->setUniform("useDiffuse", false);
		fragmentShader->setUniform("useSpecular", false);
		vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		fragmentShader->setUniform("useAmbient", false);
		fragmentShader->setUniform("useDiffuse", true);
		fragmentShader->setUniform("useSpecular", true);
		for(auto light: task.lights) {
			fragmentShader->setUniform("light.position", light.position);
			fragmentShader->setUniform("light.direction", light.direction);
			fragmentShader->setUniform("light.color", light.color);
			fragmentShader->setUniform("light.intensity", light.intensity);
			fragmentShader->setUniform("light.angle", light.angle);
			fragmentShader->setUniform("light.radius", light.radius);
			vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
		}
		glDisable(GL_BLEND);

		vao.unbind();
		pipeline.release();

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

	if(!m_shaderCache.get(task.keys.vertexShader)) {
		shared_ptr<ShaderProgram> vertexShader =
		  make_shared<ShaderProgram>(GL_VERTEX_SHADER, task.keys.vertexShader);
		m_shaderCache.put(task.keys.vertexShader, vertexShader);
	}

	if(!m_shaderCache.get(task.keys.fragmentShader)) {
		shared_ptr<ShaderProgram> fragmentShader =
		  make_shared<ShaderProgram>(GL_FRAGMENT_SHADER, task.keys.fragmentShader);
		m_shaderCache.put(task.keys.fragmentShader, fragmentShader);
	}

	m_queue.push(task);
}

unique_ptr<Texture> Renderer::loadTexture(const string& name) {
	gli::texture texture = gli::load(name);
	if(texture.empty()) {
		throw std::runtime_error(name + std::string(" is not a valid texture"));
	}

	gli::gl               gl(gli::gl::PROFILE_GL33);
	const gli::gl::format format =
	  gl.translate(texture.format(), texture.swizzles());
	GLenum target = static_cast<GLenum>(gl.translate(texture.target()));
	if(target != GL_TEXTURE_2D) {
		throw std::runtime_error(
		  "texture target is not GL_TEXTURE_2D/gli::TARGET_2D");
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
