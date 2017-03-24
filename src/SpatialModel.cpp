#include "SpatialModel.hpp"

SpatialModel* SpatialModel::activeModel = nullptr;

PREDICATE(pd_translate, 3) {
	try {
		SpatialModel::activeModel->translate(static_cast<double>(A1),
		                                     static_cast<double>(A2),
		                                     static_cast<double>(A3));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

PREDICATE(pd_rotate, 3) {
	try {
		SpatialModel::activeModel->rotate(static_cast<double>(A1),
		                                  static_cast<double>(A2),
		                                  static_cast<double>(A3));
	} catch(const PlException& exception) {
		g_logger.write(Logger::LOG_ERROR, static_cast<char*>(exception));
	}
	return true;
}

const glm::vec4 SpatialModel::canonicalForward =
  glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
const glm::vec4 SpatialModel::canonicalUp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 SpatialModel::canonicalLeft =
  glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);

SpatialModel::SpatialModel()
  : m_orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
  , m_position(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) {}

/**
 * Translates the actor along its personal axes
 *
 * @param [in] longitude  Number of units along the actor's forward/backward axis to translate by
 * @param [in] latitude   Number of units along the actor's left/right axis to translate by
 * @param [in] altitude   Number of units along the actor's up/down axis to translate by
 */
void SpatialModel::translate(float longitude, float latitude, float altitude) {
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
void SpatialModel::rotate(float roll, float pitch, float yaw) {
	glm::quat rotationQuat(glm::vec3(pitch, yaw, roll));
	m_orientation *= rotationQuat;
}

/*
 * Directly sets the actor's orientation, overriding any previous values.
 *
 * @param [in] deg  Number of degrees to rotate about the axis specified by paramaters x, y, and z
 * @param [in] x    x component of the 3D axis about which the actor will be rotated
 * @param [in] y    y component of the 3D axis about which the actor will be rotated
 * @param [in] z    z component of the 3D axis about which the actor will be rotated
 */
void SpatialModel::setOrientation(float deg, float x, float y, float z) {
	m_orientation = glm::quat(deg, x, y, z);
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
 * Return a 4-vector representing the offset from the world origin.
 *
 * @return The position vector
 */
glm::vec4 SpatialModel::position() const { return m_position; }

/**
 * Return a quaternion representing an arbitrary rotation about the local origin
 *
 * @return The rotation quaternion
 */
glm::quat SpatialModel::orientation() const { return m_orientation; }

/**
 * Returns a 4x4 matrix representing the complete world-space transformation
 *
 * @return The transformation matrix
 */
glm::mat4 SpatialModel::matrix() const {
	glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	glm::mat4 translationMatrix =
	  glm::translate(glm::mat4(), glm::vec3(m_position));

	return translationMatrix * rotationMatrix;
}
