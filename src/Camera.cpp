#include "Camera.h"

Camera::Camera() {
	
}

Camera::~Camera() {
	
}

/**
 * @return Pointer to an array of 4 values representing the camera's x, y, z, and w positions, respectively.
 *
 * TODO: Passing around pointers to arrays is actually horrible. Just throw some GLM values around for goodness' sake. A couple classes need this changed.
 */
glm::vec4 Camera::getPosition() {
	return m_position;
}

/**
 * @return Pointer to an array of 16 values representing the elements of the matrix used to transform render model with respect to the camera's position
 */
glm::mat4 Camera::getViewMatrix() {
	m_viewMatrix = glm::lookAt(glm::vec3(m_position), 
	                           glm::vec3(m_forward), 
										         glm::vec3(m_up));

	return m_viewMatrix;
}
