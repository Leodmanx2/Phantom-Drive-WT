#include "Scene.hpp"

Scene::Scene(const std::string& name) {
	// TODO: Lead scene from file
}

Scene::~Scene() {}

void Scene::simulate(std::chrono::milliseconds duration) {}

void Scene::update(std::chrono::milliseconds duration) {}

void Scene::draw() {
	// It takes a little extra computation to do this every frame instead of in
	// the renderer, but it allows for configuration of things such as the FOV
	// as part of the scene or camera.
	glm::mat4 projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(g_renderer.width()) /
	                     static_cast<float>(g_renderer.height()),
	                   0.1f,
	                   10000.0f);
	for(auto actor = m_actors.begin(); actor != m_actors.end(); ++actor) {
		for(auto shader = m_shaders.begin(); shader != m_shaders.end(); ++shader) {
			shader->setAmbience(m_ambience);
			for(size_t i = 0; i < m_pointLights.size(); ++i) {
				shader->setPointLight(i, *m_pointLights.at(i));
			}
			for(size_t i = 0; i < m_spotLights.size(); ++i) {
				shader->setSpotLight(i, *m_spotLights.at(i));
			}
			for(size_t i = 0; i < m_directionLights.size(); ++i) {
				shader->setDirectionLight(i, *m_directionLights.at(i));
			}

			// Send camera position to the GPU.
			for(auto camera : m_cameras) {
				shader->setViewMatrix(camera.getViewMatrix());
				shader->setEyePosition(glm::vec3(camera.getPosition()));
			}

			shader->setProjectionMatrix(projectionMatrix);
			shader->setObjectID(actor->first);
			actor->second->draw(
			  *shader); // TODO: Draw actor with appropriate shader, not one for each shader.
		}
	}
}

void Scene::processInput(GLFWwindow& window) {
	for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		it->second->processInput(window);
	}
	for(auto camera : m_cameras) { camera.processInput(window); }
}
