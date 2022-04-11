#ifndef PD_FRAMEBUFFER_HPP
#define PD_FRAMEBUFFER_HPP

#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

namespace PD {

	using renderbuffer_ptr = std::unique_ptr<globjects::Renderbuffer>;
	using framebuffer_ptr  = std::unique_ptr<globjects::Framebuffer>;

	class Framebuffer final {
		renderbuffer_ptr m_color_buffer;
		renderbuffer_ptr m_selection_buffer;
		renderbuffer_ptr m_depth_buffer;

		framebuffer_ptr m_frame_buffer;

		public:
		Framebuffer(renderbuffer_ptr color_buffer,
		            renderbuffer_ptr selection_buffer,
		            renderbuffer_ptr depth_buffer,
		            framebuffer_ptr  frame_buffer)
		  : m_color_buffer(std::move(color_buffer))
		  , m_selection_buffer(std::move(selection_buffer))
		  , m_depth_buffer(std::move(depth_buffer))
		  , m_frame_buffer(std::move(frame_buffer)) {}

		Framebuffer(int width, int height);

		constexpr globjects::Framebuffer* raw() const {
			return m_frame_buffer.get();
		}

		constexpr operator globjects::Framebuffer*() const { return raw(); }
	};

}; // namespace PD

#endif
