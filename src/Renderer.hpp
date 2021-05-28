#ifndef PD_RENDERER_HPP
#define PD_RENDERER_HPP

#define GLFW_INCLUDE_NONE

#include "Geometry.hpp"
#include "Light.hpp"
#include "RenderComponent.hpp"
#include "ResourceCache.hpp"
#include "ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <globjects/Renderbuffer.h>
#include <globjects/globjects.h>
#include <memory>
#include <queue>

// Forward declarations ------------------------------------------------------
class Window;
// ---------------------------------------------------------------------------

// RenderTask collects all the information required to render a game entity.
struct RenderTask {
	RenderComponent    keys;
	int                id;
	glm::mat4          model;
	glm::mat4          view;
	glm::mat4          projection;
	glm::vec3          eye;
	float              ambience;
	std::vector<Light> lights;

	RenderTask(const RenderComponent&    component,
	           int                       id,
	           glm::mat4                 model,
	           glm::mat4                 view,
	           glm::mat4                 projection,
	           glm::vec3                 eye,
	           float                     ambience,
	           const std::vector<Light>& lights);
};

class Renderer {
	private:
	std::unique_ptr<globjects::Framebuffer>  m_frameBuffer;
	std::unique_ptr<globjects::Renderbuffer> m_colorAttachment;
	std::unique_ptr<globjects::Renderbuffer> m_selectionAttachment;
	std::unique_ptr<globjects::Renderbuffer> m_depthStencilAttachment;

	ResourceCache<globjects::Texture> m_textureCache;
	ResourceCache<Geometry>           m_geometryCache;
	ResourceCache<ShaderProgram>      m_shaderCache;

	std::queue<RenderTask> m_queue;

	int m_width;
	int m_height;

	void init();
	void resize(int width, int height);
	void clear();

	std::unique_ptr<globjects::Texture> loadTexture(const std::string& name);

	public:
	Renderer(unsigned int width, unsigned int height);

	void queue(RenderTask task);
	void draw();
};

#endif
