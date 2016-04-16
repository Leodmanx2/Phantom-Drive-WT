#include "Scene.h"

//#define LOG_GL
#include "glerr.h"

Scene::Scene() {
	m_ambience = 0.1f;

	m_physicsSimulator = new PhysicsSimulator();

	m_activeCamera = new Camera();
	m_activeCamera->translate(-10.0f, 0.0f, 0.0f);

	g_logger->write(Logger::DEBUG, "Creating new DummyActor");
	m_player = new DummyActor();
	glLogErr("Constructing DummyActor");
	m_player2 = new DummyActor();
	m_player2->translate(3.0f, 3.0f, 3.0f);
	m_player3 = new DummyActor();
	m_player3->translate(-3.0f, -3.0f, 0.0f);
	m_player4 = new DummyActor();
	m_player4->translate(0.0f, 0.0f, -4.0f);

	m_activeShader = new Shader("textured.vert.glsl", "textured.frag.glsl");
	glLogErr("Constructing passthrough shader program");

	m_light = new PointLight(glm::vec3(-3.0f, 0.0f, 0.0f), // Position
	                         glm::vec3(1.0f, 0.0f, 1.0f),  // Color
	                         20.0f,                         // Intensity
	                         3.0f);                        // Radius

	m_light2 = new SpotLight(glm::vec3(2.0f, 0.0f, 0.0f),   // Position
	                          glm::vec3(0.0f, 0.0f, -1.0f), // Direction
	                          glm::vec3(0.0f, 1.0f, 0.0f),  // Color
	                          7.0f,                         // Intensity
	                          5.0f,                         // Angle
	                          15.0f);                       // Radius
}

Scene::Scene(const Scene& original) {
	m_player = new DummyActor(dynamic_cast<DummyActor&>(*original.m_player));
	m_player2 = new DummyActor(dynamic_cast<DummyActor&>(*original.m_player));
	m_player3 = new DummyActor(dynamic_cast<DummyActor&>(*original.m_player));
	m_player4 = new DummyActor(dynamic_cast<DummyActor&>(*original.m_player));
	m_light = new PointLight(*original.m_light);
	m_light2 = new SpotLight(*original.m_light2);

	m_activeCamera = new Camera(*original.m_activeCamera);
	m_physicsSimulator = new PhysicsSimulator(*original.m_physicsSimulator);
	// TODO: Shader copy (or preventing Scene copies)
}

Scene::~Scene() {
	delete m_player;
	delete m_player2;
	delete m_player3;
	delete m_player4;

	delete m_light;
	delete m_light2;


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
	m_player->update();
	m_player2->update();
	m_player3->update();
	m_player4->update();

	m_player->rotate(0.0001f, 0.0001f, 0.0f);
	m_player2->rotate(0.0001f, 0.0f, 0.0f);
	m_player3->rotate(0.0f, 0.0f, 0.00005f);
	m_player4->rotate(0.0f, 0.0001f, 0.0001f);
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

	m_activeShader->setAmbience(m_ambience);
	m_activeShader->setPointLight(0, *m_light);
	m_activeShader->setSpotLight(1, *m_light2);

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

	m_player->draw(*m_activeShader);
	m_player2->draw(*m_activeShader);
	m_player3->draw(*m_activeShader);
	m_player4->draw(*m_activeShader);
	glLogErr("Drawing actors");
}
