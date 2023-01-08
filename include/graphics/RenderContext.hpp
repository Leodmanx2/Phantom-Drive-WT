#ifndef PD_RENDERCONTEXT_HPP
#define PD_RENDERCONTEXT_HPP

#include "Framebuffer.hpp"
#include "Geometry.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "ShaderPipeline.hpp"

#include <glbinding/gl/gl.h>
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

		//template <std::input_iterator Iterator>
		template <typename Iterator>
		void highlight_pass(Iterator                      begin,
		                    Iterator                      end,
		                    const globjects::VertexArray& vao,
		                    const int                     elements) {
			glEnable(gl::GL_BLEND);
			glBlendEquation(gl::GL_FUNC_ADD);
			glBlendFunc(gl::GL_ONE, gl::GL_ONE);
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

				vao.drawElements(gl::GL_TRIANGLES, elements, gl::GL_UNSIGNED_INT);
				++begin;
			}
			glDisable(gl::GL_BLEND);
		}

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

		//template <std::input_iterator Iterator>
		template <typename Iterator>
		void draw(const textures       textures,
		          const Geometry&      geometry,
		          const int            id,
		          const mvp_transforms transforms,
		          const glm::vec3      eye,
		          const float          ambience,
		          Iterator             lights_begin,
		          Iterator             lights_end) {
			geometry.vao().bind();
			textures.albedo->bindActive(FragmentShaderProgram::ALBEDO_TEXTURE_UNIT);
			// TODO: Bind other texture units

			m_ambient_pipeline->vertex_shader().transforms(
			  transforms.model, transforms.view, transforms.projection);
			m_ambient_pipeline->fragment_shader().camera(transforms.view, eye);
			ambient_pass(geometry.vao(), geometry.elements(), ambience);

			m_highlight_pipeline->vertex_shader().transforms(
			  transforms.model, transforms.view, transforms.projection);
			m_highlight_pipeline->fragment_shader().camera(transforms.view, eye);
			highlight_pass(
			  lights_begin, lights_end, geometry.vao(), geometry.elements());
			// TODO: return ID map
		}
	};

} // namespace PD

#endif
