#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"

class Camera : public Actor {
	private:
		glm::mat4 m_viewMatrix;
	
	public:
		Camera();
		~Camera();
	
		float* getPosition();
		float* getViewMatrix();
};

#endif
