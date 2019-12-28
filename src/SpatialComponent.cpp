#include "SpatialComponent.hpp"

#include <plog/Log.h>

using namespace plog;

const glm::vec4 SpatialComponent::canonicalForward =
  glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
const glm::vec4 SpatialComponent::canonicalUp =
  glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
const glm::vec4 SpatialComponent::canonicalLeft =
  glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f);

SpatialComponent::SpatialComponent()
  : m_orientation(glm::quat(0.0f, 0.0f, 1.0f, 0.0f))
  , m_position(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)) {}

// Moving the given amount along the axes defined by m_orientation.
void SpatialComponent::translate(float longitude,
                                 float latitude,
                                 float altitude) {
	const glm::mat4 orientationMatrix = glm::mat4_cast(m_orientation);

	const glm::vec4 forward = orientationMatrix * canonicalForward;
	const glm::vec4 up      = orientationMatrix * canonicalUp;
	const glm::vec4 left    = orientationMatrix * canonicalLeft;

	const glm::vec4 longVec = forward * longitude;
	const glm::vec4 latVec  = left * latitude;
	const glm::vec4 altVec  = up * altitude;

	m_position += longVec + latVec + altVec;
}

// Rotates around the axes defined by m_orientation by the given amounts in
// degrees. Each call to rotate() updates m_orientation, so rotations do not
// compose additively.
void SpatialComponent::rotate(float roll, float pitch, float yaw) {
	const float     rollRad  = glm::radians(roll);
	const float     pitchRad = glm::radians(pitch);
	const float     yawRad   = glm::radians(yaw);
	const glm::quat rotationQuat(glm::vec3(pitchRad, yawRad, rollRad));
	m_orientation *= rotationQuat;
}

// Directly sets the orientation, overriding any previous values.
void SpatialComponent::setOrientation(float rad, float x, float y, float z) {
	m_orientation = glm::quat(rad, x, y, z);
}

// Directly sets the position, overriding any previous values.
void SpatialComponent::setPosition(float x, float y, float z) {
	m_position = glm::vec4(x, y, z, 0);
}

const glm::vec4& SpatialComponent::position() const { return m_position; }

const glm::quat& SpatialComponent::orientation() const { return m_orientation; }

// Computes a 4x4 matrix representing the complete world-space transformation
const glm::mat4 SpatialComponent::matrix() const {
	const glm::mat4 rotationMatrix = glm::mat4_cast(m_orientation);
	const glm::mat4 translationMatrix =
	  glm::translate(glm::mat4(), glm::vec3(m_position));

	return translationMatrix * rotationMatrix;
}
