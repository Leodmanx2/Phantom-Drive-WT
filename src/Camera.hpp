#ifndef CAMERA_H
#define CAMERA_H

#include "InputModel.hpp"
#include "SpatialModel.hpp"
#include "logger.hpp"
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
	glm::vec4 getPosition();
	glm::mat4 getViewMatrix();

	void processInput(GLFWwindow& window);
};

#endif
