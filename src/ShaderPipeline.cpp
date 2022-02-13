#include "ShaderPipeline.hpp"

#include <glbinding/gl/gl.h>

namespace PD {
	ShaderPipeline::ShaderPipeline(vs_ptr vertex_shader, fs_ptr fragment_shader)
	  : vertex_shader(std::move(vertex_shader))
	  , fragment_shader(std::move(fragment_shader))
	  , pipeline(new globjects::ProgramPipeline()) {
		pipeline->useStages(this->vertex_shader->raw(), gl::GL_VERTEX_SHADER_BIT);
		pipeline->useStages(this->fragment_shader->raw(),
		                    gl::GL_FRAGMENT_SHADER_BIT);
	}

} // namespace PD
