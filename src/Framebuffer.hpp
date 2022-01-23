#ifndef PD_FRAMEBUFFER_HPP
#define PD_FRAMEBUFFER_HPP

#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

namespace PD {

	using renderbuffer_ptr = std::unique_ptr<globjects::Renderbuffer>;
	using framebuffer_ptr  = std::unique_ptr<globjects::Framebuffer>;

	class Framebuffer final {
		renderbuffer_ptr color_buffer;
		renderbuffer_ptr selection_buffer;
		renderbuffer_ptr depth_buffer;

		framebuffer_ptr frame_buffer;

		public:
		Framebuffer(renderbuffer_ptr color_buffer,
		            renderbuffer_ptr selection_buffer,
		            renderbuffer_ptr depth_buffer,
		            framebuffer_ptr  frame_buffer)
		  : color_buffer(std::move(color_buffer))
		  , selection_buffer(std::move(selection_buffer))
		  , depth_buffer(std::move(depth_buffer))
		  , frame_buffer(std::move(frame_buffer)) {}

		Framebuffer(int width, int height);

		globjects::Framebuffer* raw() const;

		operator globjects::Framebuffer*() const { return raw(); }
	};

}; // namespace PD

#endif
