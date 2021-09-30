#include "Renderer.hpp"

#include "Framebuffer.hpp"

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
#include <nlohmann/json.hpp>
#include <plog/Log.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace globjects;
using namespace gl;
using namespace std;

using json = nlohmann::json;

void configure_gl() {
	// Enable back-face culling, z-buffering, and anti-aliasing
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_LINE_SMOOTH);
}

unique_ptr<PD::Framebuffer> build_framebuffer(int width, int height) {
	auto color_attachment = make_unique<Renderbuffer>();
	color_attachment->storage(GL_RGBA32F, width, height);

	auto selection_attachment = make_unique<Renderbuffer>();
	selection_attachment->storage(GL_R32UI, width, height);

	auto depth_attachment = make_unique<Renderbuffer>();
	depth_attachment->storage(GL_DEPTH24_STENCIL8, width, height);

	auto framebuffer = make_unique<Framebuffer>();
	framebuffer->attachRenderBuffer(GL_COLOR_ATTACHMENT0, color_attachment.get());
	framebuffer->attachRenderBuffer(GL_COLOR_ATTACHMENT1,
	                                selection_attachment.get());
	framebuffer->attachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT,
	                                depth_attachment.get());
	framebuffer->setDrawBuffers({GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});

	const GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(stat != GL_FRAMEBUFFER_COMPLETE) {
		throw runtime_error("could not build framebuffer");
	}

	return make_unique<PD::Framebuffer>(
	  color_attachment, selection_attachment, depth_attachment, framebuffer);
}

void prepare_framebuffer(Framebuffer& framebuffer) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	framebuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
	                  GL_STENCIL_BUFFER_BIT);
	const int color = 0;
	framebuffer.clearBuffer(GL_COLOR, 1, &color);

	framebuffer.bind(GL_DRAW_FRAMEBUFFER);
}

void draw(const json& gltf_manifest) {
	globjects::Program* vertexShader   = nullptr;
	globjects::Program* fragmentShader = nullptr;

	ProgramPipeline pipeline;
	pipeline.useStages(vertexShader, gl::GL_VERTEX_SHADER_BIT);
	pipeline.useStages(fragmentShader, gl::GL_FRAGMENT_SHADER_BIT);
	pipeline.use();

	// Upload transformation matrices
	vertexShader->setUniform("model", task.model);
	vertexShader->setUniform("view", task.view);
	vertexShader->setUniform("projection", task.projection);
	vertexShader->setUniform("normalMatrix",
	                         inverseTranspose(task.model * task.view));

	// Upload camera uniforms
	fragmentShader->setUniform("view", task.view);
	fragmentShader->setUniform("id", task.id);
	fragmentShader->setUniform("eyePos", task.eye);

	// Bind textures
	m_textureCache.get(task.keys.diffuse)->bindActive(0);
	m_textureCache.get(task.keys.specular)->bindActive(1);

	// Bind geometry
	shared_ptr<Geometry> geometry = m_geometryCache.get(task.keys.geometry);
	const VertexArray&   vao      = geometry->vao();
	int                  elements = geometry->elements();
	vao.bind();

	// Ambient pass
	fragmentShader->setUniform("useAmbient", true);
	fragmentShader->setUniform("useDiffuse", false);
	fragmentShader->setUniform("useSpecular", false);
	fragmentShader->setUniform("ambience", task.ambience);
	vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);

	// Diffuse and specular pass
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
}

void commit_frame(Framebuffer& framebuffer, GLFWwindow* window) {
	framebuffer.bind(GL_READ_FRAMEBUFFER);
	framebuffer.unbind(GL_DRAW_FRAMEBUFFER);

	const int source_width = framebuffer.getAttachmentParameter(
	  GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER_WIDTH);
	const int source_height = framebuffer.getAttachmentParameter(
	  GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER_HEIGHT);

	int destination_width, destination_height;
	glfwGetFramebufferSize(window, &destination_width, &destination_height);

	// NOTE: might be able to blit using globjects function
	//       if framebuffer object is constructed from default buffer id
	glBlitFramebuffer(0,
	                  0,
	                  source_width,
	                  source_height,
	                  0,
	                  0,
	                  destination_width,
	                  destination_height,
	                  GL_COLOR_BUFFER_BIT,
	                  GL_NEAREST);
}
