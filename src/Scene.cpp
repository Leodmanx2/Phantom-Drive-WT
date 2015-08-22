#include "Scene.h"

Scene::Scene() {
	m_activeCamera = new Camera();
	m_activeCamera->translate(-0.5f, 0.0f, 0.0f);
	
	m_player = new DummyActor();
	m_player->translate(0.0f, -256.0f, 256.0f);
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
	
}

void Scene::draw(float* projectionMatrix) {
	m_player->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
}
