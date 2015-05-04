#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include "RenderModel.h"

#include <GL/glew.h>

class Actor {
	protected:
		glm::vec3       m_position;
		glm::vec3       m_orientation;
		glm::vec3       m_forward;
		glm::vec3       m_up;
		glm::vec3       m_left;
		
		RenderModel     m_renderModel;
	
	public:
		Actor();
		virtual ~Actor();
		
		virtual void translate(float longitude, float latitude, float altitude);
		virtual void rotate(float roll, float pitch, float yaw);
		
		virtual void update();
		virtual void draw();
};

#endif
