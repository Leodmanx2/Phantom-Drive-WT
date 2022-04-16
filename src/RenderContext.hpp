#ifndef PD_RENDERCONTEXT_HPP
#define PD_RENDERCONTEXT_HPP

#include "Framebuffer.hpp"
#include "Geometry.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "ShaderPipeline.hpp"

#include <globjects/ProgramPipeline.h>
#include <globjects\VertexArray.h>
#include <iterator>
#include <memory>

namespace PD {

	class RenderContext final {
		using framebuffer_ptr = std::unique_ptr<Framebuffer>;
		using pipeline_ptr    = std::unique_ptr<ShaderPipeline>;

		std::unique_ptr<Framebuffer>    m_frame_buffer;
		std::unique_ptr<ShaderPipeline> m_ambient_pipeline;
		std::unique_ptr<ShaderPipeline> m_highlight_pipeline;

		void ambient_pass(const globjects::VertexArray& vao,
		                  const int                     elements,
		                  const float                   ambience);

		template <std::input_iterator Iterator>
		void highlight_pass(Iterator                      lights_begin,
		                    Iterator                      lights_end,
		                    const globjects::VertexArray& vao,
		                    const int                     elements);

		public:
		RenderContext(framebuffer_ptr frame_buffer,
		              pipeline_ptr    ambient_pipeline,
		              pipeline_ptr    highlight_pipeline);

		constexpr Framebuffer&    frame_buffer() const { return *m_frame_buffer; }
		constexpr ShaderPipeline& ambient_pipeline() const {
			return *m_ambient_pipeline;
		}
		constexpr ShaderPipeline& highlight_pipeline() const {
			return *m_highlight_pipeline;
		}

		template <std::input_iterator Iterator>
		void draw(const textures       albedo,
		          const Geometry&      geometry,
		          const int            id,
		          const mvp_transforms transforms,
		          const glm::vec3      eye,
		          const float          ambience,
		          Iterator             lights_begin,
		          Iterator             lights_end);
	};

} // namespace PD

#endif
