#include "Renderer.hpp"

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <globjects/ProgramPipeline.h>
#include <globjects/globjects.h>

using namespace globjects;
using namespace gl;
using namespace std;

namespace PD {

	void init_gl() {}

	void configure_gl() {
		// Enable back-face culling, z-buffering, and anti-aliasing
		glEnable(GL_CULL_FACE);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		glEnable(GL_LINE_SMOOTH);
	}

	void clear(globjects::Framebuffer& frameBuffer) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		frameBuffer.clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
		                  GL_STENCIL_BUFFER_BIT);
		const int color = 0;
		frameBuffer.clearBuffer(GL_COLOR, 1, &color);
	}

	void ambient_pass(const ShaderPipeline& pipeline,
	                  const VertexArray&    vao,
	                  const int             elements,
	                  const float           ambience) {
		pipeline.raw()->use();
		globjects::Program* vertexShader = pipeline.vs();
		vertexShader->setUniform("ambience", ambience);
		vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
	}

	template <input_iterator Iterator>
	void highlight_pass(const ShaderPipeline& pipeline,
	                    Iterator              begin,
	                    Iterator              end,
	                    const VertexArray&    vao,
	                    const int             elements) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		pipeline.raw()->use();
		globjects::Program* fragmentShader = pipeline.fs();
		while(begin != end) {
			auto light = *begin;
			fragmentShader->setUniform("light.position", light.position);
			fragmentShader->setUniform("light.direction", light.direction);
			fragmentShader->setUniform("light.color", light.color);
			fragmentShader->setUniform("light.intensity", light.intensity);
			fragmentShader->setUniform("light.angle", light.angle);
			fragmentShader->setUniform("light.radius", light.radius);

			vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
			++begin;
		}
		glDisable(GL_BLEND);
	}

	void draw(const globjects::Texture* albedo,
	          const ShaderPipeline&     ambientPipeline,
	          const ShaderPipeline&     highlightPipeline,
	          const VertexArray&        vao,
	          const int                 elements,
	          const int                 id,
	          const glm::mat4           model,
	          const glm::mat4           view,
	          const glm::mat4           projection,
	          const glm::vec3           eye,
	          const float               ambience,
	          const std::vector<Light>& lights) {
		albedo->bindActive(FragmentShaderProgram::ALBEDO_TEXTURE_UNIT);

		ambientPipeline.vs().transforms(model, view, projection);
		ambientPipeline.fs().camera(view, eye);
		ambient_pass(ambientPipeline, vao, elements, ambience);

		highlightPipeline.vs().transforms(model, view, projection);
		highlightPipeline.fs().camera(view, eye);
		highlight_pass(
		  highlightPipeline, lights.cbegin(), lights.cend(), vao, elements);
		// TODO: return ID map
	}

	void commit_frame(globjects::Framebuffer& framebuffer, GLFWwindow* window) {
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

	// TODO: Rewrite to load using globjects methods, move to appropriate file
	unique_ptr<globjects::Texture> loadTexture(const string& name) {
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

		return globjects::Texture::fromId(textureID, GL_TEXTURE_2D);
	}

} // namespace PD
