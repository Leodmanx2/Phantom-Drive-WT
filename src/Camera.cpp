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

// Forwarding functions
void Camera::translate(float longitude, float latitude, float altitude) {
	m_spatialModel.translate(longitude, latitude, altitude);
}

void Camera::rotate(float roll, float pitch, float yaw) {
	m_spatialModel.rotate(roll, pitch, yaw);
}

void Camera::setOrientation(float deg, float x, float y, float z) {
	m_spatialModel.setOrientation(deg, x, y, z);
}

void Camera::setPosition(float x, float y, float z) {
	m_spatialModel.setPosition(x, y, z);
}

const glm::vec4& Camera::position() const { return m_spatialModel.position(); }

const glm::quat& Camera::orientation() const {
	return m_spatialModel.orientation();
}
