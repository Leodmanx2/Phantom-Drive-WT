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
	SpatialModel::activeModel = &m_spatialModel;
	InputModel::activeModel   = &m_inputModel;
	m_inputModel.process(event);
	SpatialModel::activeModel = nullptr;
	InputModel::activeModel   = nullptr;
}

void Camera::process(const MouseButtonEvent& event) {
	SpatialModel::activeModel = &m_spatialModel;
	InputModel::activeModel   = &m_inputModel;
	m_inputModel.process(event);
	SpatialModel::activeModel = nullptr;
	InputModel::activeModel   = nullptr;
}

void Camera::process(const MouseMovementEvent& event) {
	SpatialModel::activeModel = &m_spatialModel;
	InputModel::activeModel   = &m_inputModel;
	m_inputModel.process(event);
	SpatialModel::activeModel = nullptr;
	InputModel::activeModel   = nullptr;
}
