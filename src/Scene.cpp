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

	m_shaders.at(0).setAmbience(m_ambience);
	m_shaders.at(0).setPointLight(0, *m_pointLights.at(0));
	m_shaders.at(0).setSpotLight(0, *m_spotLights.at(0));
	m_shaders.at(0).setDirectionLight(0, *m_directionLights.at(0));

	// Send camera position to the GPU.
	m_shaders.at(0).setViewMatrix(m_cameras.at(0).getViewMatrix());
	m_shaders.at(0).setEyePosition(glm::vec3(m_cameras.at(0).getPosition()));
	m_shaders.at(0).setProjectionMatrix(projectionMatrix);

	for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		m_shaders.at(0).setObjectID(it->first);
		it->second->draw(m_shaders.at(0));
	}
}

void Scene::processInput(GLFWwindow& window) {
	for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		it->second->processInput(window);
	}
	m_cameras.at(0).processInput(window);
}
