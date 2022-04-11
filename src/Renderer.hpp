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

	void init_gl();

	void configure_gl();

	void clear(globjects::Framebuffer& frameBuffer);

	void commit_frame(Framebuffer& framebuffer, GLFWwindow* window);

	std::unique_ptr<globjects::Texture> loadTexture(const std::string& name);

} // namespace PD

#endif
