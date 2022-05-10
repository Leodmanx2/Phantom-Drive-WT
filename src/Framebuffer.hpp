#ifndef PD_FRAMEBUFFER_HPP
#define PD_FRAMEBUFFER_HPP

#include <globjects/Framebuffer.h>
#include <globjects/Renderbuffer.h>

namespace PD {

	class Framebuffer final {
		using renderbuffer_ptr = std::unique_ptr<globjects::Renderbuffer>;
		using framebuffer_ptr  = std::unique_ptr<globjects::Framebuffer>;

		renderbuffer_ptr color_buffer;
		renderbuffer_ptr selection_buffer;
		renderbuffer_ptr depth_buffer;

		framebuffer_ptr frame_buffer;

		public:
		const int width;
		const int height;

		Framebuffer(int width, int height);

		constexpr globjects::Framebuffer* raw() const { return frame_buffer.get(); }

		constexpr operator globjects::Framebuffer*() const { return raw(); }
	};

}; // namespace PD

#endif
