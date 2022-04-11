#ifndef PD_PIPELINE_HPP
#define PD_PIPELINE_HPP

#include "ShaderProgram.hpp"

#include <globjects/ProgramPipeline.h>

namespace PD {

	class ShaderPipeline final {
		using vs_ptr       = std::unique_ptr<VertexShaderProgram>;
		using fs_ptr       = std::unique_ptr<FragmentShaderProgram>;
		using pipeline_ptr = std::unique_ptr<globjects::ProgramPipeline>;

		vs_ptr       m_vertex_shader;
		fs_ptr       m_fragment_shader;
		pipeline_ptr m_pipeline;

		public:
		ShaderPipeline(vs_ptr vertex_shader, fs_ptr fragment_shader);

		constexpr VertexShaderProgram& vertex_shader() const {
			return *m_vertex_shader;
		}
		constexpr FragmentShaderProgram& fragment_shader() const {
			return *m_fragment_shader;
		}
		constexpr globjects::ProgramPipeline* raw() const {
			return m_pipeline.get();
		}

		constexpr operator globjects::ProgramPipeline*() const { return raw(); }
	};

} // namespace PD

#endif
