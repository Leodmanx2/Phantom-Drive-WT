#include "Camera.h"

Camera::Camera() {
	
}

Camera::~Camera() {
	
}

float* Camera::getPosition() {
	return glm::value_ptr(m_position);
}

float* Camera::getViewMatrix() {
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), 
	                           glm::vec3(m_forward), 
										         glm::vec3(m_up));

	return glm::value_ptr(m_viewMatrix);
}
