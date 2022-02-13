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

	void clear(globjects::Framebuffer* frameBuffer);

	void update_transforms(globjects::Program* vertexShader,
	                       const glm::mat4     model,
	                       const glm::mat4     view,
	                       const glm::mat4     projection);

	void update_camera(globjects::Program* fragmentShader,
	                   const glm::mat4     view,
	                   const glm::vec3     eye);

	void use_textures(globjects::Program*       fragmentShader,
	                  const globjects::Texture* diffuse,
	                  const globjects::Texture* specular);

	void ambient_pass(const ShaderPipeline&         pipeline,
	                  const globjects::VertexArray& vao,
	                  const int                     elements,
	                  const float                   ambience);

	template <std::input_iterator Iterator>
	void highlight_pass(const ShaderPipeline&         pipeline,
	                    Iterator                      begin,
	                    Iterator                      end,
	                    const globjects::VertexArray& vao,
	                    const int                     elements);

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
