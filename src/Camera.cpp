#include "Camera.hpp"

Camera::Camera() {
	try {
		// TODO: Consult with file in memory
		//PlCall("consult", PlTerm("keys"));
		PlCall("pd_consult", PlTermv("camIOHandle", "keys.pro"));
		PlCall("b_setval", PlTermv("pd_input", &m_inputModel));
		PlCall("bindKeys");
		PlCall("bindMouse");
		PlCall("b_setval", PlTermv("pd_input", static_cast<long>(NULL)));
	} catch(const PlException& exception) {
		g_logger->write(Logger::LOG_ERROR, static_cast<char*>(exception));
	}
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

void Camera::processInput(GLFWwindow& window) {
	PlCall("b_setval", PlTermv("pd_spatial", &m_spatialModel));
	PlCall("b_setval", PlTermv("pd_input", &m_inputModel));
	m_inputModel.update(window);
	PlCall("b_setval", PlTermv("pd_spatial", static_cast<long>(NULL)));
	PlCall("b_setval", PlTermv("pd_input", static_cast<long>(NULL)));
}
