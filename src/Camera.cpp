#include "Camera.hpp"

PREDICATE(pd_translate, 3) {
	try {
		PlTerm modelTerm;
		PlCall("b_getval", PlTermv("pd_spatial", modelTerm));
		SpatialModel* model =
		  static_cast<SpatialModel*>(static_cast<void*>(modelTerm));
		model->translate(static_cast<double>(A1),
		                 static_cast<double>(A2),
		                 static_cast<double>(A3));
	} catch(const PlException& exception) {
		std::cerr << static_cast<char*>(exception) << std::endl;
	}
	return true;
}

Camera::Camera() {
	try {
		PlCall("consult", PlTerm("keys"));
		PlCall("b_setval", PlTermv("pd_spatial", &m_spatialModel));
		PlCall("process", PlTerm(static_cast<long>(87)));
	} catch(const PlException& exception) {
		std::cerr << static_cast<char*>(exception) << std::endl;
	}

	// TODO: Should be defined dynamically
	m_inputModel.bindKey(GLFW_KEY_W, [&]() {
		PlCall("process", PlTerm(static_cast<long>(GLFW_KEY_W)));
	});

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
