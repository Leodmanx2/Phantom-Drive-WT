#ifndef PD_RENDERER_HPP
#define PD_RENDERER_HPP

#define GLFW_INCLUDE_NONE

#include "Framebuffer.hpp"
#include "Geometry.hpp"
#include "Light.hpp"
#include "ShaderPipeline.hpp"
#include "ShaderProgram.hpp"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <globjects\Program.h>
#include <globjects\Texture.h>
#include <globjects\VertexArray.h>
#include <iterator>
#include <memory>

namespace PD {

	struct mvp_transforms {
		const glm::mat4 model;
		const glm::mat4 view;
		const glm::mat4 projection;
	};

	struct textures {
		const globjects::Texture* albedo;
	};

	void configure_gl();

	std::unique_ptr<Framebuffer> init_framebuffer(int width, int height);

	void clear(globjects::Framebuffer& frameBuffer);

	void commit_frame(Framebuffer& framebuffer, GLFWwindow* window);

	std::unique_ptr<globjects::Texture> load_texture(const std::string& name);

} // namespace PD

#endif
