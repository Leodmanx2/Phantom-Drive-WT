#include "ShaderPipeline.hpp"

#include <glbinding/gl/gl.h>

namespace PD {
	ShaderPipeline::ShaderPipeline(vs_ptr vertex_shader, fs_ptr fragment_shader)
	  : m_vertex_shader(vertex_shader)
	  , m_fragment_shader(fragment_shader)
	  , m_pipeline(new globjects::ProgramPipeline()) {
		m_pipeline->useStages(m_vertex_shader->raw(), gl::GL_VERTEX_SHADER_BIT);
		m_pipeline->useStages(m_fragment_shader->raw(), gl::GL_FRAGMENT_SHADER_BIT);
	}

} // namespace PD
