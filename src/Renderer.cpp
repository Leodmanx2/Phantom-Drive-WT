#include "Renderer.hpp"

#include <glbinding/gl/gl.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <globjects/Texture.h>

using namespace globjects;
using namespace gl;

void Renderer::draw() {
	while(!m_queue.empty()) {
		RenderTask task = m_queue.front();

		auto shader = m_shaderCache.get(task.keys.shader);

		shader->setUniform("id", task.id);
		shader->setUniform("model", task.model);
		shader->setUniform("view", task.view);
		shader->setUniform("projection", task.projection);
		shader->setUniform("eyePos", task.eye);
		shader->setUniform("ambience", task.ambience);
		shader->setUniform("normal", inverseTranspose(task.model * task.view));

		m_textureCache.get(task.keys.diffuse)->bindActive(0);
		m_textureCache.get(task.keys.specular)->bindActive(1);

		auto vao = m_geometryCache.get(task.keys.geometry);
		vao->bind();
		vao->drawElements(GL_TRIANGLES, task.keys.elementCount, GL_UNSIGNED_INT);
		vao->unbind();

		m_queue.pop();
	}
}
