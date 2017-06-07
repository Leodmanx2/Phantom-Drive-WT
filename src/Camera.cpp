#include "Camera.hpp"

Camera::Camera() : m_inputModel("camera") {}

const glm::mat4 Camera::viewMatrix() const {
	const glm::mat4 orientationMatrix =
	  glm::mat4_cast(m_spatialModel.orientation());

	const glm::vec4 forward = orientationMatrix * SpatialModel::canonicalForward;
	const glm::vec4 up      = orientationMatrix * SpatialModel::canonicalUp;

	const glm::mat4 viewMatrix =
	  glm::lookAt(glm::vec3(m_spatialModel.position()),
	              glm::vec3(m_spatialModel.position() + forward),
	              glm::vec3(up));

	return viewMatrix;
}

void Camera::processInput(GLFWwindow& window) {
	SpatialModel::activeModel = &m_spatialModel;
	InputModel::activeModel   = &m_inputModel;
	m_inputModel.update(window);
	SpatialModel::activeModel = nullptr;
	InputModel::activeModel   = nullptr;
}
