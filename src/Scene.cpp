#include "Scene.hpp"

Scene::Scene() {
	m_ambience = 0.2f;

	try {
		m_actors.emplace_back(std::make_unique<Actor>("Akari", ++m_highestID));
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load Actor");
	}

	m_activeCamera = new Camera();

	m_activeShader = new Shader("textured.vert.glsl", "textured.frag.glsl");

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
	delete m_pointLight;
	delete m_spotLight;
	delete m_directionLight;

	delete m_activeCamera;
	delete m_activeShader;
}

void Scene::update(std::chrono::milliseconds duration) {}

void Scene::draw() {
	// It takes a little extra computation to do this every frame instead of in
	// the renderer, but it allows for configuration of things such as the FOV
	// as part of the scene or camera.
	glm::mat4 projectionMatrix =
	  glm::perspective(45.0f,
	                   static_cast<float>(g_renderer.width()) /
	                     static_cast<float>(g_renderer.height()),
	                   0.1f,
	                   10000.0f);

	m_activeShader->setAmbience(m_ambience);
	m_activeShader->setPointLight(0, *m_pointLight);
	m_activeShader->setSpotLight(0, *m_spotLight);
	m_activeShader->setDirectionLight(0, *m_directionLight);

	// Send camera position to the GPU.
	m_activeShader->setViewMatrix(m_activeCamera->getViewMatrix());
	m_activeShader->setEyePosition(glm::vec3(m_activeCamera->getPosition()));
	m_activeShader->setProjectionMatrix(projectionMatrix);

	m_actors.at(0)->draw(*m_activeShader);
}

void Scene::processInput(GLFWwindow& window) {
	for(auto it = m_actors.begin(); it != m_actors.end(); ++it) {
		(*it)->processInput(window);
	}
	m_activeCamera->processInput(window);
}
