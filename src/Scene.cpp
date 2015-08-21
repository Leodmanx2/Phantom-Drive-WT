#include "Scene.h"

Scene::Scene(const char* sceneFile) {
	m_activeCamera = new Camera();
	m_activeCamera->translate(-0.5f, 0.0f, 0.0f);
	
	m_actor = new DummyActor();
	m_actor->translate(0.0f, -256.0f, 256.0f);
}

Scene::~Scene() {
	delete m_actor;
	delete m_activeCamera;
}

void Scene::update() {
	
}

void Scene::draw(float* projectionMatrix) {
	m_actor->draw(m_activeCamera->getViewMatrix(), projectionMatrix);
}
