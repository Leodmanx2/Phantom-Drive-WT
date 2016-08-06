#include "Actor.hpp"

std::map<std::string, std::shared_ptr<RenderModel>> Actor::s_modelDictionary;

const glm::vec4 Actor::canonicalForward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
const glm::vec4 Actor::canonicalUp      = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 Actor::canonicalLeft    = glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);

Actor::Actor(const std::string& actorName)
  : m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
  , m_position(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
  , name(actorName) {
	if(name.compare("") != 0) {
		// TODO: Review construction of Actor without model
		try {
			// TODO: Model name neds to be read from Actor description file
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
}

Actor::~Actor() {
	// Remove model from dictionary if no Actor is using it
	if(name.compare("") != 0 && s_modelDictionary.find(name)->second.unique()) {
		s_modelDictionary.erase(name);
	}
}

/**
 * Translates the actor along its personal axes
 *
 * @param [in] longitude  Number of units along the actor's forward/backward axis to translate by
 * @param [in] latitude   Number of units along the actor's left/right axis to translate by
 * @param [in] altitude   Number of units along the actor's up/down axis to translate by
 */
void Actor::translate(float longitude, float latitude, float altitude) {
	glm::mat4 orientationMatrix = glm::mat4_cast(m_orientation);

	glm::vec4 forward = orientationMatrix * canonicalForward;
	glm::vec4 up      = orientationMatrix * canonicalUp;
	glm::vec4 left    = orientationMatrix * canonicalLeft;

	glm::vec4 longVec = forward * longitude;
	glm::vec4 latVec  = left * latitude;
	glm::vec4 altVec  = up * altitude;

	m_position += longVec + latVec + altVec;
}

/**
 * Rotates the actor around its personal axes
 *
 * @param [in] roll   Number of degrees around the forward/backward axis to rotate by
 * @param [in] pitch  Number of degrees around the left/right axis to rotate by
 * @param [in] yaw    Number of degrees around the up/dawn axis to rotate by
 */
void Actor::rotate(float roll, float pitch, float yaw) {
	glm::quat rotationQuat(glm::vec3(pitch, yaw, roll));
	m_orientation *= rotationQuat;
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

	glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	glm::mat4 translationMatrix =
	  glm::translate(glm::mat4(), glm::vec3(m_position));

	glm::mat4 modelMatrix = translationMatrix * rotationMatrix;

	shader.setModelMatrix(modelMatrix);

	shader.updateNormalMatrix();

	m_renderModel->draw(shader);
}

/*
 * Directly sets the actor's orientation, overriding any previous values.
 *
 * @param [in] deg  Number of degrees to rotate about the axis specified by paramaters x, y, and z
 * @param [in] x    x component of the 3D axis about which the actor will be rotated
 * @param [in] y    y component of the 3D axis about which the actor will be rotated
 * @param [in] z    z component of the 3D axis about which the actor will be rotated
 */
void Actor::setOrientation(float deg, float x, float y, float z) {
	m_orientation = glm::quat(deg, x, y, z);
}

/*
 * Directly sets the actor's position, overriding any previous values.
 *
 * @param [in] x  Absolute x position in world space
 * @param [in] y  Absolute y position in world space
 * @param [in] z  Absolute z position in world space
 */
void Actor::setPosition(float x, float y, float z) {
	m_position = glm::vec4(x, y, z, 0);
}
