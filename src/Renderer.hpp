#ifndef RENDERER_H
#define RENDERER_H

#include "ResourceCache.hpp"
#include <glm/glm.hpp>
#include <globjects/globjects.h>
#include <memory>
#include <queue>
#include <string>

struct RenderComponent {
	std::string diffuse;
	std::string specular;
	std::string shader;
	std::string geometry;
	int         elementCount;
};

struct RenderTask {
	RenderComponent keys;
	int             id;
	glm::mat4       model;
	glm::mat4       view;
	glm::mat4       projection;
	glm::vec4       eye;
	float           ambience;
	// other lights
};

class Renderer {
	private:
	std::unique_ptr<globjects::Framebuffer>  m_frameBuffer;
	std::unique_ptr<globjects::Renderbuffer> m_colorAttachment;
	std::unique_ptr<globjects::Renderbuffer> m_depthStencilAttachment;

	ResourceCache<globjects::Texture>     m_textureCache;
	ResourceCache<globjects::VertexArray> m_geometryCache;
	ResourceCache<globjects::Program>     m_shaderCache;

	std::queue<RenderTask> m_queue;

	public:
	Renderer();

	void queue(RenderTask);
	void draw();
};

#endif
