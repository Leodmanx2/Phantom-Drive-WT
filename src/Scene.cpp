#include "Scene.hpp"

//#define LOG_GL
#include "glerr.hpp"

Scene::Scene() {
	m_ambience = 0.1f;

	m_physicsSimulator = new PhysicsSimulator();

	m_activeCamera = new Camera();
	m_activeCamera->translate(-10.0f, 0.0f, 0.0f);

	try {
		g_logger->write(Logger::DEBUG, "Making render model");
		// This will be read from an Actor description file
		const std::string modelName = "Akari";

		// Construct model if it has not been constructed yet
		if(m_renderModels.count(modelName) == 0) {
			m_renderModels.emplace(modelName,
			                       std::make_shared<RenderModel>(modelName));
		}

		g_logger->write(Logger::DEBUG, "Creating new Actor");
		m_actors.emplace_back(new Actor(m_renderModels.find(modelName)->second));
		glLogErr("Constructing Actor");
	} catch(const std::exception& exception) {
		g_logger->write(Logger::ERROR, exception.what());
		throw std::runtime_error("Failed to load RenderModel");
	}

	m_activeShader = new Shader("textured.vert.glsl", "textured.frag.glsl");
	glLogErr("Constructing passthrough shader program");

	m_pointLight = new PointLight(glm::vec3(-3.0f, 0.0f, 0.0f), // Position
	                              glm::vec3(1.0f, 0.0f, 0.0f),  // Color
	                              20.0f,                        // Intensity
	                              3.0f);                        // Radius

	m_spotLight = new SpotLight(glm::vec3(2.0f, 0.0f, 0.0f),  // Position
	                            glm::vec3(0.0f, 0.0f, -1.0f), // Direction
	                            glm::vec3(0.0f, 1.0f, 0.0f),  // Color
	                            7.0f,                         // Intensity
	                            5.0f,                         // Angle
	                            15.0f);                       // Radius

	m_directionLight =
	  new DirectionLight(glm::vec3(1.0f, 0.0f, 0.0f), // Direction
	                     glm::vec3(0.0f, 0.0f, 1.0f), // Color
	                     1.0f);                       // Intensity
}

Scene::~Scene() {
	m_actors.clear();

	delete m_pointLight;
	delete m_spotLight;
	delete m_directionLight;

	delete m_activeCamera;
	delete m_physicsSimulator;
	delete m_activeShader;
}

/**
 * Called to perform once-per-cycle processing
 *
 * TODO: Need to take time since last cycle as input
 */
void Scene::update() { m_actors.at(0)->rotate(0.0001f, 0.0001f, 0.0f); }

/**
 * Runs the once-per-cycle physics simulation. Likely to be merged into the default update() method.
 *
 * TODO: Needs to take time since last cycle as input
 */
void Scene::simulate() { m_physicsSimulator->stepSimulation(); }

/**
 * Draws all render models that are part of the scene using information passed down by the renderer
 *
 * @param [in] projectionMatrix  Pointer to a 16-element array representing the 3D->2D, world->screen transformation
 */
void Scene::draw(glm::mat4 projectionMatrix) {
	glLogErr("Pre-draw check");

	m_activeShader->setAmbience(m_ambience);
	m_activeShader->setPointLight(0, *m_pointLight);
	m_activeShader->setSpotLight(0, *m_spotLight);
	m_activeShader->setDirectionLight(0, *m_directionLight);

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

	m_actors.at(0)->draw(*m_activeShader);
	glLogErr("Drawing actors");
}
