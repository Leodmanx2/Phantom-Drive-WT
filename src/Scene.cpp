#include "Scene.h"

Scene::Scene() {
	m_physicsSimulator = new PhysicsSimulator();
	
	m_activeCamera = new Camera();
	m_activeCamera->translate(-1000.0f, 0.0f, 0.0f);
	
	m_player = new DummyActor(m_physicsSimulator);
	m_player2 = new DummyActor(m_physicsSimulator);
	
	g_logger->write(Logger::DEBUG, "Creating new DummyActor2");
	m_player3 = new DummyActor2();
	
	m_player2->translate(0.0f, 400.0f, 0.0f);
	m_player3->translate(-990.0f, 0.0f, 0.0f);
	
	
}

Scene::Scene(const Scene& original) {
	m_player = new Actor(*original.m_player);
	m_player2 = new Actor(*original.m_player2);
	m_player2 = new Actor(*original.m_player3);
	m_activeCamera = new Camera(*original.m_activeCamera);
	m_physicsSimulator = new PhysicsSimulator(*original.m_physicsSimulator);
}

Scene::~Scene() {
	delete m_player;
	delete m_player2;
	delete m_player3;
	delete m_activeCamera;
	delete m_physicsSimulator;
}

/**
 * Called to perform once-per-cycle processing
 *
 * TODO: Need to take time since last cycle as input
 */
void Scene::update() {
	m_player->update();
	m_player2->update();
	m_player3->update();
	
	m_player->rotate(0.001f, 0.0f, 0.0f);
	m_player2->translate(0.0f, 0.2f, 0.0f);
	m_player3->rotate(0.0001f, 0.0001f, 0.0f);
}

/**
 * Runs the once-per-cycle physics simulation. Likely to be merged into the default update() method.
 *
 * TODO: Needs to take time since last cycle as input
 */
void Scene::simulate() {
	m_physicsSimulator->stepSimulation();
}

/**
 * Draws all render models that are part of the scene using information passed down by the renderer
 *
 * @param [in] projectionMatrix  Pointer to a 16-element array representing the 3D->2D, world->screen transformation
 */
void Scene::draw(glm::mat4 projectionMatrix) {
	// Send camera position to the GPU.
	// NOTE: Lighting uses explicit uniform locations. eyePos must be declared at 
	//       location 0 in the shader, and lights from 1-8 (while we use forward 
	//       rendering)
	glm::vec4 camPos4 = m_activeCamera->getPosition();
	glm::vec3 camPos3 = glm::vec3(camPos4.x, camPos4.y, camPos4.z);
	glUniform3fv(0, 1, glm::value_ptr(camPos3));
	
	m_player->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
	m_player2->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
	m_player3->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
}
