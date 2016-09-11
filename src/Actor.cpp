#include "Actor.hpp"

std::map<std::string, std::shared_ptr<RenderModel>> Actor::s_modelDictionary;

Actor::Actor(const std::string& actorName) : name(actorName) {
	assert(actorName.compare("") != 0);
	try {
		// TODO: Model name needs to be read from Actor description file
		// For the time being, we're assuming it's the same as the Actor's name.

		// Construct model if it has not been constructed yet
		if(s_modelDictionary.count(name) == 0) {
			s_modelDictionary.emplace(name, std::make_shared<RenderModel>(name));
		}
		m_renderModel = s_modelDictionary.find(name)->second;
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load RenderModel");
	}
}

Actor::~Actor() {
	// Remove model from dictionary if no Actor is using it
	if(name.compare("") != 0 && s_modelDictionary.find(name)->second.unique()) {
		s_modelDictionary.erase(name);
	}
}

/**
 * Called once per cycle. By default does nothing, but can be overriden to
 * progress animations, respond to collisions, etc.
 *
 * TODO: Allow user definition via scripting
 *
 * @param [in] duration   Time in milliseconds since the last update
 */
// void Actor::update(std::chrono::milliseconds duration) {}

/* Draws the actor's render model to the active frame buffer
 *
 * @param [in] viewMatrix        The matrix used to transform the model respective to a camera's viewpoint
 * @param [in] projectionMatrix  The matrix used to project the model from 3D/4D world space to 2D screen space
 */
void Actor::draw(Shader& shader) {
	if(m_renderModel == nullptr) return;

	glm::mat4 modelMatrix = m_spatialModel.matrix();

	shader.setModelMatrix(modelMatrix);

	shader.updateNormalMatrix();

	m_renderModel->draw(shader);
}

void Actor::processInput(GLFWwindow& window) { m_inputModel.update(window); }
