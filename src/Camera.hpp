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

	public:
	Camera();
	const glm::mat4 viewMatrix() const;

	constexpr const glm::vec4& position() const {
		return m_spatialModel.position();
	}

	constexpr const glm::quat& orientation() const {
		return m_spatialModel.orientation();
	}

	void processInput(GLFWwindow& window);
};

#endif
