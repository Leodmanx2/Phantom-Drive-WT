#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"

class Camera : public Actor {
	private:
		glm::mat4 m_viewMatrix;
	
	public:
		Camera();
		~Camera();
	
		glm::vec4 getPosition();
		glm::mat4 getViewMatrix();
};

#endif
