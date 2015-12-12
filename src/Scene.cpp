#include "Scene.h"

#define LOG_GL
#include "glerr.h"

Scene::Scene() {
	m_physicsSimulator = new PhysicsSimulator();
	
	m_activeCamera = new Camera();
	m_activeCamera->translate(-1000.0f, 0.0f, 0.0f);
	
	//m_player = new DummyActor(m_physicsSimulator);
	//m_player2 = new DummyActor(m_physicsSimulator);
	glLogErr("Constructing DummyActors");
	
	g_logger->write(Logger::DEBUG, "Creating new DummyActor2");
	m_player3 = new DummyActor2();
	glLogErr("Constructing DummyActor2");
	
	//m_player2->translate(0.0f, 400.0f, 0.0f);
	m_player3->translate(-990.0f, 0.0f, 0.0f);
	
	m_activeShader = new Shader("passthrough.vert.glsl", "passthrough.frag.glsl");
	glLogErr("Constructing passthrough shader program");
}

Scene::Scene(const Scene& original) {
	//m_player = new Actor(*original.m_player);
	//m_player2 = new Actor(*original.m_player2);
	m_player3 = new DummyActor2(dynamic_cast<DummyActor2&>(*original.m_player3));
	m_activeCamera = new Camera(*original.m_activeCamera);
	m_physicsSimulator = new PhysicsSimulator(*original.m_physicsSimulator);
	// TODO: Shader copy (or preventing Scene copies)
}

Scene::~Scene() {
	//delete m_player;
	//delete m_player2;
	delete m_player3;
	delete m_activeCamera;
	delete m_physicsSimulator;
	delete m_activeShader;
}

/**
 * Called to perform once-per-cycle processing
 *
 * TODO: Need to take time since last cycle as input
 */
void Scene::update() {
	//m_player->update();
	//m_player2->update();
	m_player3->update();
	
	//m_player->rotate(0.001f, 0.0f, 0.0f);
	//m_player2->translate(0.0f, 0.2f, 0.0f);
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
	glLogErr("Pre-draw check");
	
	// Send camera position to the GPU.
	// NOTE: Lighting uses explicit uniform locations. eyePos must be declared at 
	//       location 0 in the shader, and lights from 1-8 (while we use forward 
	//       rendering)
	glm::vec4 camPos4 = m_activeCamera->getPosition();
	glm::vec3 camPos3 = glm::vec3(camPos4.x, camPos4.y, camPos4.z);

	m_activeShader->setViewMatrix(m_activeCamera->getViewMatrix());
	glLogErr("Uploading view matrix");
	
	m_activeShader->setEyePosition(camPos3);
	glLogErr("Uploading camera position");
	
	m_activeShader->setProjectionMatrix(projectionMatrix);
	glLogErr("Uploading projection matrix");
	
	//m_player->draw(*m_activeShader);
	//m_player2->draw(*m_activeShader);
	m_player3->draw(*m_activeShader);
	glLogErr("Drawing actors");
}