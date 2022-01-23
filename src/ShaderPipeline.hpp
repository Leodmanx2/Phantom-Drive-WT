#ifndef PD_PIPELINE_HPP
#define PD_PIPELINE_HPP

#include "ShaderProgram.hpp"

#include <globjects/ProgramPipeline.h>

namespace PD {

	class ShaderPipeline final {
		using shader_ptr   = std::unique_ptr<ShaderProgram>;
		using pipeline_ptr = std::unique_ptr<globjects::ProgramPipeline>;

		shader_ptr   vertex_shader;
		shader_ptr   fragment_shader;
		pipeline_ptr pipeline;

		public:
		ShaderPipeline(shader_ptr vertex_shader, shader_ptr fragment_shader);

		globjects::Program*         vs() const;
		globjects::Program*         fs() const;
		globjects::ProgramPipeline* raw() const;

		operator globjects::ProgramPipeline*() const { return raw(); }
	};

} // namespace PD

#endif
