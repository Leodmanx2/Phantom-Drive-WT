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

	// NOTE: Will take place at application level
	void commit_frame(const Framebuffer& framebuffer, GLFWwindow* window) {
		int destination_width, destination_height;
		glfwGetFramebufferSize(window, &destination_width, &destination_height);

		auto default_framebuffer = globjects::Framebuffer::defaultFBO();

		framebuffer.raw()->blit(GL_BACK,
		                        {0, 0, framebuffer.width, framebuffer.height},
		                        default_framebuffer.get(),
		                        GL_BACK,
		                        {0, 0, destination_width, destination_height},
		                        GL_COLOR_BUFFER_BIT,
		                        GL_NEAREST);
	}

	// TODO: Rewrite to load using globjects methods, move to appropriate file
	unique_ptr<globjects::Texture> load_texture(const string& name) {
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
