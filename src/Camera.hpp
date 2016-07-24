#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.hpp"

class Camera : public Actor {
	private:
	glm::mat4 m_viewMatrix;

	public:
	glm::vec4 getPosition();
	glm::mat4 getViewMatrix();
};

#endif
