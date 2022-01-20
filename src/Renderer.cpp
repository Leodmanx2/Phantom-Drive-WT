#include "Renderer.hpp"

#include <GLFW/glfw3.h>
#include <glbinding/gl/gl.h>
#include <gli/gli.hpp>
#include <glm/gtc/matrix_inverse.hpp>
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

	void clear(globjects::Framebuffer* frameBuffer) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		frameBuffer->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
		                   GL_STENCIL_BUFFER_BIT);
		const int color = 0;
		frameBuffer->clearBuffer(GL_COLOR, 1, &color);
	}

	std::unique_ptr<ProgramPipeline>
	use_shaders(globjects::Program* vertexShader,
	            globjects::Program* fragmentShader) {
		auto pipeline = make_unique<ProgramPipeline>();
		pipeline->useStages(vertexShader, gl::GL_VERTEX_SHADER_BIT);
		pipeline->useStages(fragmentShader, gl::GL_FRAGMENT_SHADER_BIT);
		pipeline->use();
		return pipeline;
	}

	void update_transforms(globjects::Program* vertexShader,
	                       const glm::mat4     model,
	                       const glm::mat4     view,
	                       const glm::mat4     projection) {
		vertexShader->setUniform("model", model);
		vertexShader->setUniform("view", view);
		vertexShader->setUniform("projection", projection);
		vertexShader->setUniform("normalMatrix", inverseTranspose(model * view));
	}

	void update_camera(globjects::Program* fragmentShader,
	                   const glm::mat4     view,
	                   const glm::vec3     eye) {
		fragmentShader->setUniform("view", view);
		fragmentShader->setUniform("eyePos", eye);
	}

	void use_textures(globjects::Program*       fragmentShader,
	                  const globjects::Texture* diffuse,
	                  const globjects::Texture* specular) {
		fragmentShader->setUniform("diffuseMap", 0);
		diffuse->bindActive(0);
		fragmentShader->setUniform("specularMap", 1);
		specular->bindActive(1);
	}

	void ambient_pass(globjects::Program* vertexShader,
	                  globjects::Program* ambientShader,
	                  const VertexArray&  vao,
	                  const int           elements,
	                  const float         ambience) {
		auto pipeline = use_shaders(vertexShader, ambientShader);
		ambientShader->setUniform("ambience", ambience);
		vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
	}

	template <input_iterator Iterator>
	void highlight_pass(globjects::Program* highlightShader,
	                    Iterator            begin,
	                    Iterator            end,
	                    const VertexArray&  vao,
	                    const int           elements) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		while(begin != end) {
			auto light = *begin;
			highlightShader->setUniform("light.position", light.position);
			highlightShader->setUniform("light.direction", light.direction);
			highlightShader->setUniform("light.color", light.color);
			highlightShader->setUniform("light.intensity", light.intensity);
			highlightShader->setUniform("light.angle", light.angle);
			highlightShader->setUniform("light.radius", light.radius);

			vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
			++begin;
		}
		glDisable(GL_BLEND);
	}

	void draw(const globjects::Texture* diffuse,
	          const globjects::Texture* specular,
	          globjects::Program*       vertexShader,
	          globjects::Program*       ambientShader,
	          globjects::Program*       highlightShader,
	          const VertexArray&        vao,
	          const int                 elements,
	          const int                 id,
	          const glm::mat4           model,
	          const glm::mat4           view,
	          const glm::mat4           projection,
	          const glm::vec3           eye,
	          const float               ambience,
	          const std::vector<Light>& lights) {
		update_transforms(vertexShader, model, view, projection);
		update_camera(ambientShader, view, eye);
		use_textures(ambientShader, diffuse, specular);
		ambient_pass(ambientShader, vao, elements, ambience);

		update_camera(highlightShader, view, eye);
		use_textures(highlightShader, diffuse, specular);
		highlight_pass(
		  highlightShader, lights.cbegin(), lights.cend(), vao, elements);
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
