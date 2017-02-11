#include "EditorScene.hpp"

EditorScene::EditorScene(const std::string& name)
  : Scene(name), m_inputModel("SceneEdit") {
	// The scene will load normally first.
	// If the scene file can't be found, we create a new one.
	// If the scene file is corrupt, we give up.

	// TODO: The below should be removed and replaced with PSCN data

	m_ambience = 0.2f;

	try {
		m_actors.emplace(
		  std::make_pair(++m_highestID, std::make_unique<Actor>("Akari")));
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load Actor");
	}

	m_cameras.emplace_back();

	m_shaders.emplace_back("textured.vert.glsl", "textured.frag.glsl");

	m_pointLights[0] =
	  std::make_unique<PointLight>(glm::vec3(-3.0f, 0.0f, 0.0f), // Position
	                               glm::vec3(1.0f, 0.0f, 0.0f),  // Color
	                               20.0f,                        // Intensity
	                               3.0f);                        // Radius

	m_spotLights[0] =
	  std::make_unique<SpotLight>(glm::vec3(2.0f, 0.0f, 0.0f),  // Position
	                              glm::vec3(0.0f, 0.0f, -1.0f), // Direction
	                              glm::vec3(0.0f, 1.0f, 0.0f),  // Color
	                              7.0f,                         // Intensity
	                              5.0f,                         // Angle
	                              15.0f);                       // Radius

	m_directionLights[0] =
	  std::make_unique<DirectionLight>(glm::vec3(1.0f, 0.0f, 0.0f), // Direction
	                                   glm::vec3(0.0f, 0.0f, 1.0f), // Color
	                                   1.0f);                       // Intensity
}

// The editor works with the initial moment of a scene.
// We don't want these functions to do anything.
void EditorScene::update(std::chrono::milliseconds) {}
void EditorScene::simulate(std::chrono::milliseconds) {}

// Similarly, we don't want to Actors processing any input.
// Input processing is limited to editor and camera input.
void EditorScene::processInput(GLFWwindow& window) {
	m_inputModel.update(window);
	m_cameras.at(0).processInput(window);
}

// The scene should draw as per usual, though.
void EditorScene::draw() { Scene::draw(); }
