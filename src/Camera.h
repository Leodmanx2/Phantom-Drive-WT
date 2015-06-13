#ifndef CAMERA_H
#define CAMERA_H

#include "Actor.h"

class Camera : public Actor {
	public:
		Camera();
		~Camera();
	
		float* getPosition();
		float* getViewMatrix();
};

#endif
