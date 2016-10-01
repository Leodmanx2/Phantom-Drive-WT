#include "Camera.hpp"

Camera::Camera() : m_inputModel("camera") {}

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

void Camera::processInput(GLFWwindow& window) {
	SpatialModel::activeModel = &m_spatialModel;
	InputModel::activeModel   = &m_inputModel;
	m_inputModel.update(window);
	SpatialModel::activeModel = nullptr;
	InputModel::activeModel   = nullptr;
}
