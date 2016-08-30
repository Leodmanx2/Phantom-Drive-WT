#include "Camera.hpp"

Camera::Camera() {}

glm::vec4 Camera::getPosition() { return m_position.position(); }

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 orientationMatrix = glm::mat4_cast(m_position.orientation());

	glm::vec4 forward = orientationMatrix * SpatialModel::canonicalForward;
	glm::vec4 up      = orientationMatrix * SpatialModel::canonicalUp;

	m_viewMatrix = glm::lookAt(glm::vec3(m_position.position()),
	                           glm::vec3(m_position.position() + forward),
	                           glm::vec3(up));

	return m_viewMatrix;
}

void Camera::translate(float longitude, float latitude, float altitude) {
	m_position.translate(longitude, latitude, altitude);
}

void Camera::rotate(float roll, float pitch, float yaw) {
	m_position.rotate(roll, pitch, yaw);
}
