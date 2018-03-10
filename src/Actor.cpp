#include "Actor.hpp"

#include "Logger.hpp"
#include "RenderModel.hpp"
#include "Shader.hpp"
#include "utility.hpp"

PREDICATE(pd_translate_actor, 4) {
	try {
		Actor& actor = *static_cast<Actor*>(static_cast<void*>(A1));
		actor.translate(static_cast<double>(A2),
		                static_cast<double>(A3),
		                static_cast<double>(A4));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

PREDICATE(pd_rotate_actor, 4) {
	try {
		Actor& actor = *static_cast<Actor*>(static_cast<void*>(A1));
		actor.rotate(static_cast<double>(A2),
		             static_cast<double>(A3),
		             static_cast<double>(A4));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

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
	m_inputModel.process(m_spatialModel, event);
}

void Actor::process(const MouseButtonEvent& event) {
	m_inputModel.process(m_spatialModel, event);
}

void Actor::process(const MouseMovementEvent& event) {
	m_inputModel.process(m_spatialModel, event);
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

// Forwarding functions
void Actor::translate(float longitude, float latitude, float altitude) {
	m_spatialModel.translate(longitude, latitude, altitude);
}

void Actor::rotate(float roll, float pitch, float yaw) {
	m_spatialModel.rotate(roll, pitch, yaw);
}

void Actor::setOrientation(float deg, float x, float y, float z) {
	m_spatialModel.setOrientation(deg, x, y, z);
}

void Actor::setPosition(float x, float y, float z) {
	m_spatialModel.setPosition(x, y, z);
}

const glm::vec4& Actor::position() const { return m_spatialModel.position(); }

const glm::quat& Actor::orientation() const {
	return m_spatialModel.orientation();
}
