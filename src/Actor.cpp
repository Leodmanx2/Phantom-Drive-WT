#include "Actor.hpp"

std::map<std::string, std::shared_ptr<RenderModel>> Actor::s_modelDictionary;

Actor::Actor(const std::string& actorName)
  : m_desc(actorName), m_inputModel(m_desc.inputModel) {
	if(m_desc.renderModel.compare("") != 0) {
		try {
			// Construct model if it has not been constructed yet
			if(s_modelDictionary.count(m_desc.renderModel) == 0) {
				s_modelDictionary.emplace(
				  m_desc.renderModel,
				  std::make_shared<RenderModel>(m_desc.renderModel));
			}
			m_renderModel = s_modelDictionary.find(m_desc.renderModel)->second;
		} catch(const std::exception& exception) {
			g_logger.write(Logger::LogLevel::LOG_ERROR, exception.what());
			throw std::runtime_error("Failed to load RenderModel");
		}
	}
}

Actor::~Actor() {
	// Remove model from dictionary if no Actor is using it
	m_renderModel.reset();
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

	const glm::mat4 modelMatrix = m_spatialModel.matrix();
	shader.setModelMatrix(modelMatrix);
	shader.updateNormalMatrix();

	m_renderModel->draw(shader);
}

void Actor::process(const KeyEvent& event) {
	SpatialModel::activeModel = &m_spatialModel;
	m_inputModel.process(event);
	SpatialModel::activeModel = nullptr;
}

void Actor::process(const MouseButtonEvent& event) {
	SpatialModel::activeModel = &m_spatialModel;
	m_inputModel.process(event);
	SpatialModel::activeModel = nullptr;
}

void Actor::process(const MouseMovementEvent& event) {
	SpatialModel::activeModel = &m_spatialModel;
	m_inputModel.process(event);
	SpatialModel::activeModel = nullptr;
}

const std::string& Actor::name() const { return m_desc.name; }

Actor::ActorDescription::ActorDescription(const std::string& actorName)
  : name(actorName) {
	assert(actorName.compare("") != 0);
	// TODO: Actor file error checking
	std::string        contents = readFile(ACTOR_DIR + actorName + ".actr");
	std::istringstream ss(contents);
	std::getline(ss, renderModel);
	std::getline(ss, inputModel);
}
