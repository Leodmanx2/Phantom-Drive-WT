#include "Camera.hpp"

Camera::Camera() : Actor("") {}

glm::vec4 Camera::getPosition() { return m_position; }

glm::mat4 Camera::getViewMatrix() {
	glm::mat4 orientationMatrix = glm::mat4_cast(m_orientation);

	glm::vec4 forward = orientationMatrix * canonicalForward;
	glm::vec4 up      = orientationMatrix * canonicalUp;

	m_viewMatrix = glm::lookAt(
	  glm::vec3(m_position), glm::vec3(m_position + forward), glm::vec3(up));

	return m_viewMatrix;
}
