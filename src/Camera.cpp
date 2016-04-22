#include "Camera.h"

glm::vec4 Camera::getPosition() {
	return m_position;
}

glm::mat4 Camera::getViewMatrix() {
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), 
	                           glm::vec3(m_forward), 
										         glm::vec3(m_up));

	return m_viewMatrix;
}
