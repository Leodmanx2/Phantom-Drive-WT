#include "RenderContext.hpp"

#include <glbinding/gl/gl.h>

using namespace std;
using namespace gl;
using namespace globjects;

namespace PD {

	RenderContext::RenderContext(framebuffer_ptr frame_buffer,
	                             pipeline_ptr    ambient_pipeline,
	                             pipeline_ptr    highlight_pipeline)
	  : m_frame_buffer(std::move(frame_buffer))
	  , m_ambient_pipeline(std::move(ambient_pipeline))
	  , m_highlight_pipeline(std::move(highlight_pipeline)) {}

	void RenderContext::ambient_pass(const globjects::VertexArray& vao,
	                                 const int                     elements,
	                                 const float                   ambience) {
		m_ambient_pipeline->raw()->use();
		globjects::Program& vertexShader = *m_ambient_pipeline->vertex_shader();
		vertexShader.setUniform("ambience", ambience);
		vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
	}

	template <input_iterator Iterator>
	void RenderContext::highlight_pass(Iterator           begin,
	                                   Iterator           end,
	                                   const VertexArray& vao,
	                                   const int          elements) {
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);
		m_highlight_pipeline->raw()->use();
		globjects::Program& fragment_shader =
		  *m_highlight_pipeline->fragment_shader();
		while(begin != end) {
			auto light = *begin;
			fragment_shader.setUniform("light.position", light.position);
			fragment_shader.setUniform("light.direction", light.direction);
			fragment_shader.setUniform("light.color", light.color);
			fragment_shader.setUniform("light.intensity", light.intensity);
			fragment_shader.setUniform("light.angle", light.angle);
			fragment_shader.setUniform("light.radius", light.radius);

			vao.drawElements(GL_TRIANGLES, elements, GL_UNSIGNED_INT);
			++begin;
		}
		glDisable(GL_BLEND);
	}

	template <std::input_iterator Iterator>
	void RenderContext::draw(const globjects::Texture* albedo,
	                         const Geometry&           geometry,
	                         const int                 id,
	                         const glm::mat4           model,
	                         const glm::mat4           view,
	                         const glm::mat4           projection,
	                         const glm::vec3           eye,
	                         const float               ambience,
	                         Iterator                  begin,
	                         Iterator                  end) {
		albedo->bindActive(FragmentShaderProgram::ALBEDO_TEXTURE_UNIT);

		m_ambient_pipeline->vertex_shader().transforms(model, view, projection);
		m_ambient_pipeline->fragment_shader().camera(view, eye);
		ambient_pass(geometry.vao(), geometry.elements(), ambience);

		// TODO: Bind other texture units

		m_highlight_pipeline->vertex_shader().transforms(model, view, projection);
		m_highlight_pipeline->fragment_shader().camera(view, eye);
		highlight_pass(begin(), end(), geometry.vao(), geometry.elements());
		// TODO: return ID map
	}

} // namespace PD
