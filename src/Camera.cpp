#include "Camera.hpp"

Camera::Camera() {
	// TODO: Should be defined dynamically
	m_inputModel.bindKey(GLFW_KEY_W,
	                     [&]() { m_spatialModel.translate(1.0f, 0.0f, 0.0f); });

	m_inputModel.bindKey(GLFW_KEY_A,
	                     [&]() { m_spatialModel.translate(0.0f, 1.0f, 0.0f); });

	m_inputModel.bindKey(GLFW_KEY_S,
	                     [&]() { m_spatialModel.translate(-1.0f, 0.0f, 0.0f); });

	m_inputModel.bindKey(GLFW_KEY_D,
	                     [&]() { m_spatialModel.translate(0.0f, -1.0f, 0.0f); });

	m_inputModel.bindKey(GLFW_KEY_Z,
	                     [&]() { m_spatialModel.translate(0.0f, 0.0f, 1.0f); });

	m_inputModel.bindKey(GLFW_KEY_X,
	                     [&]() { m_spatialModel.translate(0.0f, 0.0f, -1.0f); });

	m_inputModel.bindMouse([&](glm::dvec2 lastPos, glm::dvec2 newPos) {
		m_spatialModel.rotate(0.0f,
		                      -((newPos.y - lastPos.y) / 128.0f),
		                      -((newPos.x - lastPos.x) / 128.0f));
	});
}

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

void Camera::processInput(GLFWwindow& window) { m_inputModel.update(window); }
