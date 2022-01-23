#include "ShaderPipeline.hpp"

namespace PD {
	ShaderPipeline::ShaderPipeline(shader_ptr vertex_shader,
	                               shader_ptr fragment_shader)
	  : vertex_shader(std::move(vertex_shader))
	  , fragment_shader(std::move(fragment_shader)) {}

	globjects::Program* ShaderPipeline::vs() const {
		return vertex_shader->raw();
	}

	globjects::Program* ShaderPipeline::fs() const {
		return fragment_shader->raw();
	}

	globjects::ProgramPipeline* ShaderPipeline::raw() const {
		return pipeline.get();
	}

} // namespace PD
