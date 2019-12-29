#include "SpatialComponent.hpp"

#include <plog/Log.h>

using namespace plog;
using namespace glm;

const vec4 SpatialComponent::canonicalForward = vec4(0.0f, 0.0f, -1.0f, 0.0f);
const vec4 SpatialComponent::canonicalUp      = vec4(0.0f, 1.0f, 0.0f, 0.0f);
const vec4 SpatialComponent::canonicalLeft    = vec4(-1.0f, 0.0f, 0.0f, 0.0f);

SpatialComponent::SpatialComponent()
  : m_orientation(quat(0.0f, 0.0f, 1.0f, 0.0f))
  , m_position(vec4(0.0f, 0.0f, 0.0f, 0.0f)) {}

// Moving the given amount along the axes defined by m_orientation.
void SpatialComponent::translate(float longitude,
                                 float latitude,
                                 float altitude) {
	const mat4 orientationMatrix = mat4_cast(m_orientation);

	const vec4 forward = orientationMatrix * canonicalForward;
	const vec4 up      = orientationMatrix * canonicalUp;
	const vec4 left    = orientationMatrix * canonicalLeft;

	const vec4 longVec = forward * longitude;
	const vec4 latVec  = left * latitude;
	const vec4 altVec  = up * altitude;

	m_position += longVec + latVec + altVec;
}

// Rotates around the axes defined by m_orientation by the given amounts in
// degrees. Each call to rotate() updates m_orientation, so rotations do not
// compose additively.
void SpatialComponent::rotate(float roll, float pitch, float yaw) {
	const float rollRad  = radians(roll);
	const float pitchRad = radians(pitch);
	const float yawRad   = radians(yaw);
	const quat  rotationQuat(vec3(pitchRad, yawRad, rollRad));
	m_orientation *= rotationQuat;
}

// Directly sets the orientation, overriding any previous values.
void SpatialComponent::setOrientation(float rad, float x, float y, float z) {
	m_orientation = quat(rad, x, y, z);
}

// Directly sets the position, overriding any previous values.
void SpatialComponent::setPosition(float x, float y, float z) {
	m_position = vec4(x, y, z, 0);
}

const vec4& SpatialComponent::position() const { return m_position; }

const quat& SpatialComponent::orientation() const { return m_orientation; }

// Computes a 4x4 matrix representing the complete world-space transformation
const mat4 SpatialComponent::matrix() const {
	const mat4 rotationMatrix    = mat4_cast(m_orientation);
	const mat4 translationMatrix = glm::translate(mat4(), vec3(m_position));

	return translationMatrix * rotationMatrix;
}
