#ifndef CAMERA_H
#define CAMERA_H

#include "InputModel.hpp"
#include "Logger.hpp"
#include "SpatialModel.hpp"
#include <SWI-cpp.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>

class Camera {
	private:
	InputModel   m_inputModel;
	SpatialModel m_spatialModel;
	glm::mat4    m_viewMatrix;

	public:
	Camera();
	glm::vec4 getPosition(); // TODO: Normalize spatial model forwarding functions
	inline const glm::quat orientation() const {
		return m_spatialModel.orientation();
	}
	glm::mat4 getViewMatrix();

	void processInput(GLFWwindow& window);
};

#endif
