#ifndef CAMERA_H
#define CAMERA_H

#include "SpatialModel.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
	private:
	SpatialModel m_spatialModel;
	glm::mat4    m_viewMatrix;

	public:
	Camera();
	glm::vec4 getPosition();
	glm::mat4 getViewMatrix();
};

#endif
