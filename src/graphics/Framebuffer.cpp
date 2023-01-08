#include "Framebuffer.hpp"

#include <glbinding/gl/gl.h>

using namespace gl;
using namespace std;
using namespace globjects;

namespace PD {

	Framebuffer::Framebuffer(int width, int height)
	  : color_buffer(new Renderbuffer())
	  , selection_buffer(new Renderbuffer())
	  , depth_buffer(new Renderbuffer())
	  , frame_buffer(new globjects::Framebuffer())
	  , width(width)
	  , height(height) {
		color_buffer->storage(GL_RGBA32F, width, height);
		selection_buffer->storage(GL_R32UI, width, height);
		depth_buffer->storage(GL_DEPTH24_STENCIL8, width, height);

		frame_buffer->attachRenderBuffer(GL_COLOR_ATTACHMENT0, color_buffer.get());
		frame_buffer->attachRenderBuffer(GL_COLOR_ATTACHMENT1,
		                                 selection_buffer.get());
		frame_buffer->attachRenderBuffer(GL_DEPTH_STENCIL_ATTACHMENT,
		                                 depth_buffer.get());
		frame_buffer->setDrawBuffers({GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1});

		const GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(stat != GL_FRAMEBUFFER_COMPLETE) {
			throw runtime_error("could not build framebuffer");
		}
	}

} // namespace PD
