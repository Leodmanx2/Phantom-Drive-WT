#include "Scene.h"

Scene::Scene() {
	m_physicsSimulator = new PhysicsSimulator();
	
	m_activeCamera = new Camera();
	m_activeCamera->translate(-1000.0f, 0.0f, 0.0f);
	
	m_player = new DummyActor(m_physicsSimulator);
	m_player2 = new DummyActor(m_physicsSimulator);
	
	m_player2->translate(0.0f, 400.0f, 0.0f);
}

Scene::Scene(const Scene& original) {
	m_player = new Actor(*original.m_player);
	m_player2 = new Actor(*original.m_player);
	m_activeCamera = new Camera(*original.m_activeCamera);
	m_physicsSimulator = new PhysicsSimulator(*original.m_physicsSimulator);
}

Scene::~Scene() {
	delete m_player;
	delete m_player2;
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
	
	m_player->rotate(0.001f, 0.0f, 0.0f);
	m_player2->translate(0.0f, 0.2f, 0.0f);
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
void Scene::draw(float* projectionMatrix) {
	m_player->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
	m_player2->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
}
