#include "RenderContext.hpp"

#include "ShaderProgram.hpp"

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

} // namespace PD
