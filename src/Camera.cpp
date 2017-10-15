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

void Camera::process(const KeyEvent& event) {
	m_inputModel.process(m_spatialModel, event);
}

void Camera::process(const MouseButtonEvent& event) {
	m_inputModel.process(m_spatialModel, event);
}

void Camera::process(const MouseMovementEvent& event) {
	m_inputModel.process(m_spatialModel, event);
}
