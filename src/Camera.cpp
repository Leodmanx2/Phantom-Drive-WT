#include "Camera.hpp"

Camera::Camera() {
	try {
		// TODO: Consult with file in memory
		PlCall("consult", PlTerm("keys"));
		PlCall("b_setval", PlTermv("pd_input", &m_inputModel));
		PlCall("bindKeys");
		PlCall("b_setval", PlTermv("pd_input", static_cast<long>(NULL)));
	} catch(const PlException& exception) {
		std::cerr << static_cast<char*>(exception) << std::endl;
	}

	// TODO: Define dynamically
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

void Camera::processInput(GLFWwindow& window) {
	PlCall("b_setval", PlTermv("pd_spatial", &m_spatialModel));
	PlCall("b_setval", PlTermv("pd_input", &m_inputModel));
	m_inputModel.update(window);
	PlCall("b_setval", PlTermv("pd_spatial", static_cast<long>(NULL)));
	PlCall("b_setval", PlTermv("pd_input", static_cast<long>(NULL)));
}
