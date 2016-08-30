#include "Camera.hpp"

Camera::Camera() {}

glm::vec4 Camera::getPosition() { return m_spatialModel.position(); }

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 orientationMatrix = glm::mat4_cast(m_spatialModel.orientation());

	glm::vec4 forward = orientationMatrix * SpatialModel::canonicalForward;
	glm::vec4 up      = orientationMatrix * SpatialModel::canonicalUp;

	m_viewMatrix = glm::lookAt(glm::vec3(m_spatialModel.position()),
	                           glm::vec3(m_spatialModel.position() + forward),
	                           glm::vec3(up));

	return m_viewMatrix;
}

void Camera::translate(float longitude, float latitude, float altitude) {
	m_spatialModel.translate(longitude, latitude, altitude);
}

void Camera::rotate(float roll, float pitch, float yaw) {
	m_spatialModel.rotate(roll, pitch, yaw);
}
