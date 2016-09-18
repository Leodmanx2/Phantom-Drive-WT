#include "Actor.hpp"

std::map<std::string, std::shared_ptr<RenderModel>> Actor::s_modelDictionary;

Actor::Actor(const std::string& actorName) : m_desc(actorName) {
	assert(actorName.compare("") != 0);
	try {
		// Construct model if it has not been constructed yet
		if(s_modelDictionary.count(m_desc.renderModel) == 0) {
			s_modelDictionary.emplace(
			  m_desc.renderModel, std::make_shared<RenderModel>(m_desc.renderModel));
		}
		m_renderModel = s_modelDictionary.find(m_desc.renderModel)->second;
	} catch(const std::exception& exception) {
		g_logger->write(Logger::LOG_ERROR, exception.what());
		throw std::runtime_error("Failed to load RenderModel");
	}

	// Consult input schema file and create bindings
	if(m_desc.inputModel.compare("") != 0) {
		try {
			const std::string inputSchemaName = m_desc.inputModel + ".pro";
			std::string       keyPred         = m_desc.inputModel + "_bindKeys";
			PlCall("pd_consult", PlTerm(inputSchemaName.c_str()));
			PlCall("b_setval", PlTermv("pd_input", &m_inputModel));
			PlCall(keyPred.c_str(), NULL);
			PlCall("pd_bindMouse", PlTerm((&m_desc.inputModel)->c_str()));
			PlCall("b_setval", PlTermv("pd_input", static_cast<long>(NULL)));
		} catch(const PlException& exception) {
			g_logger->write(Logger::LOG_ERROR, static_cast<char*>(exception));
		}
	}
}

Actor::~Actor() {
	// Remove model from dictionary if no Actor is using it
	if(m_desc.renderModel.compare("") != 0 &&
	   s_modelDictionary.find(m_desc.renderModel)->second.unique()) {
		s_modelDictionary.erase(m_desc.renderModel);
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

void Actor::processInput(GLFWwindow& window) {
	PlCall("b_setval", PlTermv("pd_spatial", &m_spatialModel));
	PlCall("b_setval", PlTermv("pd_input", &m_inputModel));
	m_inputModel.update(window);
	PlCall("b_setval", PlTermv("pd_spatial", static_cast<long>(NULL)));
	PlCall("b_setval", PlTermv("pd_input", static_cast<long>(NULL)));
}

ActorDescription::ActorDescription(const std::string& actorName) {
	std::string        contents = readFile(ACTOR_DIR + actorName + ".actr");
	std::istringstream ss(contents);
	std::getline(ss, renderModel);
	std::getline(ss, inputModel);
}
