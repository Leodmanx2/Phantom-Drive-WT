#include "Scene.hpp"

#include "Actor.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
#include "input.hpp"

Scene::Scene(const std::string& name) : m_ambience(1.0f), m_highestID(0) {
	// TODO: Load scene from file
}

Scene::~Scene() {}

void Scene::simulate(const std::chrono::milliseconds& duration) {}

void Scene::update(const std::chrono::milliseconds& duration) {}

void Scene::draw() {
	// It takes a little extra computation to do this every frame instead of in
	// the renderer, but it allows for configuration of things such as the FOV
	// as part of the scene or camera.
	const glm::mat4 projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(Renderer::width()) /
	                     static_cast<float>(Renderer::height()),
	                   0.1f,
	                   10000.0f);
	for(auto actor = m_actors.begin(); actor != m_actors.end(); ++actor) {
		for(auto shader = m_shaders.begin(); shader != m_shaders.end(); ++shader) {
			shader->setAmbience(m_ambience);
			for(std::size_t i = 0; i < m_pointLights.size(); ++i) {
				shader->setPointLight(i, *m_pointLights.at(i));
			}
			for(std::size_t i = 0; i < m_spotLights.size(); ++i) {
				shader->setSpotLight(i, *m_spotLights.at(i));
			}
			for(std::size_t i = 0; i < m_directionLights.size(); ++i) {
				shader->setDirectionLight(i, *m_directionLights.at(i));
			}

			// Send camera position to the GPU.
			for(auto camera : m_cameras) {
				shader->setViewMatrix(camera.viewMatrix());
				shader->setEyePosition(glm::vec3(camera.position()));
			}

			shader->setProjectionMatrix(projectionMatrix);
			shader->setObjectID(actor->first);
			actor->second->draw(
			  *shader); // TODO: Draw actor with appropriate shader, not one for each shader.
		}
	}
}

void Scene::process(std::queue<KeyEvent>& keyEvents) {
	while(!keyEvents.empty()) {
		for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
			it->second->process(keyEvents.front());
		}
		for(auto camera : m_cameras) { camera.process(keyEvents.front()); }
		keyEvents.pop();
	}
}

void Scene::process(std::queue<MouseButtonEvent>& buttonEvents) {
	while(!buttonEvents.empty()) {
		for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
			it->second->process(buttonEvents.front());
		}
		for(auto camera : m_cameras) { camera.process(buttonEvents.front()); }
		buttonEvents.pop();
	}
}

void Scene::process(std::queue<MouseMovementEvent>& movementEvents) {
	while(!movementEvents.empty()) {
		for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
			it->second->process(movementEvents.front());
		}
		for(auto camera : m_cameras) { camera.process(movementEvents.front()); }
		movementEvents.pop();
	}
}
