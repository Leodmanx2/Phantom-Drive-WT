#include "SpatialModel.hpp"

PREDICATE(pd_translate, 4) {
	try {
		SpatialModel& model = *static_cast<SpatialModel*>(static_cast<void*>(A1));
		model.translate(static_cast<double>(A2),
		                static_cast<double>(A3),
		                static_cast<double>(A4));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

PREDICATE(pd_rotate, 4) {
	try {
		SpatialModel& model = *static_cast<SpatialModel*>(static_cast<void*>(A1));
		model.rotate(static_cast<double>(A2),
		             static_cast<double>(A3),
		             static_cast<double>(A4));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LogLevel::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

const glm::vec4 SpatialModel::canonicalForward =
  glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
const glm::vec4 SpatialModel::canonicalUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 SpatialModel::canonicalLeft =
  glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);

SpatialModel::SpatialModel()
  : m_orientation(glm::quat(0.0f, 0.0f, 1.0f, 0.0f))
  , m_position(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) {}

/**
 * Translates the actor along its personal axes
 *
 * @param [in] longitude  Number of units along the actor's forward/backward axis to translate by
 * @param [in] latitude   Number of units along the actor's left/right axis to translate by
 * @param [in] altitude   Number of units along the actor's up/down axis to translate by
 */
void SpatialModel::translate(float longitude, float latitude, float altitude) {
	const glm::mat4 orientationMatrix = glm::mat4_cast(m_orientation);

	const glm::vec4 forward = orientationMatrix * canonicalForward;
	const glm::vec4 up      = orientationMatrix * canonicalUp;
	const glm::vec4 left    = orientationMatrix * canonicalLeft;

	const glm::vec4 longVec = forward * longitude;
	const glm::vec4 latVec  = left * latitude;
	const glm::vec4 altVec  = up * altitude;

	m_position += longVec + latVec + altVec;
}

/**
 * Rotates the actor around its personal axes
 *
 * @param [in] roll   Number of degrees around the forward/backward axis to rotate by
 * @param [in] pitch  Number of degrees around the left/right axis to rotate by
 * @param [in] yaw    Number of degrees around the up/dawn axis to rotate by
 */
void SpatialModel::rotate(float roll, float pitch, float yaw) {
	const float     rollRad  = glm::radians(roll);
	const float     pitchRad = glm::radians(pitch);
	const float     yawRad   = glm::radians(yaw);
	const glm::quat rotationQuat(glm::vec3(pitchRad, yawRad, rollRad));
	m_orientation *= rotationQuat;
}

/*
 * Directly sets the actor's orientation, overriding any previous values.
 *
 * @param [in] rad  Number of radians to rotate about the axis specified by paramaters x, y, and z
 * @param [in] x    x component of the 3D axis about which the actor will be rotated
 * @param [in] y    y component of the 3D axis about which the actor will be rotated
 * @param [in] z    z component of the 3D axis about which the actor will be rotated
 */
void SpatialModel::setOrientation(float rad, float x, float y, float z) {
	m_orientation = glm::quat(rad, x, y, z);
}

/*
 * Directly sets the actor's position, overriding any previous values.
 *
 * @param [in] x  Absolute x position in world space
 * @param [in] y  Absolute y position in world space
 * @param [in] z  Absolute z position in world space
 */
void SpatialModel::setPosition(float x, float y, float z) {
	m_position = glm::vec4(x, y, z, 0);
}

/**
 * Returns a 4x4 matrix representing the complete world-space transformation
 *
 * @return The transformation matrix
 */
const glm::mat4 SpatialModel::matrix() const {
	const glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	const glm::mat4 translationMatrix =
	  glm::translate(glm::mat4(), glm::vec3(m_position));

	return translationMatrix * rotationMatrix;
}
