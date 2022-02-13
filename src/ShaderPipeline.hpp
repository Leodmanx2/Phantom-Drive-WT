#ifndef PD_PIPELINE_HPP
#define PD_PIPELINE_HPP

#include "ShaderProgram.hpp"

#include <globjects/ProgramPipeline.h>

namespace PD {

	class ShaderPipeline final {
		using vs_ptr       = std::unique_ptr<VertexShaderProgram>;
		using fs_ptr       = std::unique_ptr<FragmentShaderProgram>;
		using pipeline_ptr = std::unique_ptr<globjects::ProgramPipeline>;

		vs_ptr       vertex_shader;
		fs_ptr       fragment_shader;
		pipeline_ptr pipeline;

		public:
		ShaderPipeline(vs_ptr vertex_shader, fs_ptr fragment_shader);

		constexpr VertexShaderProgram&   vs() const { return *vertex_shader; }
		constexpr FragmentShaderProgram& fs() const { return *fragment_shader; }
		constexpr globjects::ProgramPipeline* raw() const { return pipeline.get(); }

		constexpr operator globjects::ProgramPipeline*() const { return raw(); }
	};

} // namespace PD

#endif
