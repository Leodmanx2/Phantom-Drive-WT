#ifndef PD_RENDERER_HPP
#define PD_RENDERER_HPP

#define GLFW_INCLUDE_NONE

#include "Framebuffer.hpp"
#include "Geometry.hpp"
#include "Light.hpp"

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <globjects\Program.h>
#include <globjects\Texture.h>
#include <memory>

namespace PD {

	void init_gl();

	void configure_gl();

	std::unique_ptr<Framebuffer> build_framebuffer(int width, int height);

	void prepare_framebuffer(Framebuffer& framebuffer);

	void draw(const globjects::Texture* diffuse,
	          const globjects::Texture* specular,
	          globjects::Program*       vertexShader,
	          globjects::Program*       fragmentShader,
	          const Geometry&           geometry,
	          const int                 id,
	          const glm::mat4           model,
	          const glm::mat4           view,
	          const glm::mat4           projection,
	          const glm::vec3           eye,
	          const float               ambience,
	          const std::vector<Light>& lights);

	void commit_frame(Framebuffer& framebuffer, GLFWwindow* window);

	std::unique_ptr<globjects::Texture> loadTexture(const std::string& name);

} // namespace PD

#endif
