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

void Scene::update() {
	m_player->update();
	m_player2->update();
	
	m_player->rotate(0.001f, 0.0f, 0.0f);
	m_player2->translate(0.0f, 0.2f, 0.0f);
}

void Scene::simulate() {
	m_physicsSimulator->stepSimulation();
}

void Scene::draw(float* projectionMatrix) {
	m_player->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
	m_player2->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
}
