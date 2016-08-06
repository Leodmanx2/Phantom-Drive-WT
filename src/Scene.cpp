#include "Scene.hpp"

Scene::Scene() {
	m_ambience = 0.2f;

	try {
		m_actors.emplace_back(std::make_unique<Actor>("Akari"));
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load Actor");
	}

	m_physicsSimulator = new PhysicsSimulator();

	m_activeCamera = new Camera();
	m_activeCamera->translate(-10.0f, 0.0f, 0.0f);

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
	delete m_physicsSimulator;
	delete m_activeShader;
}

/**
 * Called to perform once-per-cycle processing
 *
 * @param [in] duration   Time in milliseconds since the last simulation step
 */
void Scene::update(std::chrono::milliseconds duration) {}

/**
 * Runs the once-per-cycle physics simulation. Likely to be merged into the default update() method.
 *
 * @param [in] duration   Time in milliseconds since the last update
 */
void Scene::simulate(std::chrono::milliseconds duration) {
	m_physicsSimulator->stepSimulation(duration);
}

/**
 * Draws all render models that are part of the scene using information passed down by the renderer
 *
 * @param [in] projectionMatrix  Pointer to a 16-element array representing the 3D->2D, world->screen transformation
 */
void Scene::draw(glm::mat4 projectionMatrix) {
	// TODO: Fix light moving with camera
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
	if(glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
		m_activeCamera->translate(1.0f, 0.0f, 0.0f);
	if(glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
		m_activeCamera->translate(0.0f, 1.0f, 0.0f);
	if(glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
		m_activeCamera->translate(-1.0f, 0.0f, 0.0f);
	if(glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
		m_activeCamera->translate(0.0f, -1.0f, 0.0f);
	if(glfwGetKey(&window, GLFW_KEY_Z) == GLFW_PRESS)
		m_activeCamera->translate(0.0f, 0.0f, 1.0f);
	if(glfwGetKey(&window, GLFW_KEY_X) == GLFW_PRESS)
		m_activeCamera->translate(0.0f, 0.0f, -1.0f);

	static glm::dvec2 lastPos;
	glm::dvec2        newPos;
	glfwGetCursorPos(&window, &newPos.x, &newPos.y);
	m_activeCamera->rotate(0.0f,
	                       -((newPos.y - lastPos.y) / 128.0f),
	                       -((newPos.x - lastPos.x) / 128.0f));
	lastPos = newPos;
}
