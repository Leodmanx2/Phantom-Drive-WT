#include "Scene.h"

Scene::Scene() {
	m_activeCamera = new Camera();
	
	m_player = new DummyActor();
	m_player->translate(1000.0f, -256.0f, 256.0f);
	m_player->rotate(0.0f, 0.0f, 0.5f);
}

Scene::Scene(const Scene& original) {
	m_player = new Actor(*original.m_player);
	m_activeCamera = new Camera(*original.m_activeCamera);
}

Scene::~Scene() {
	delete m_player;
	delete m_activeCamera;
}

void Scene::update() {
	m_player->rotate(0.0f, 0.0001f, 0.0f);
}

void Scene::draw(float* projectionMatrix) {
	m_player->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
}
