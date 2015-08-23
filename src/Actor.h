#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderModel.h"

#include <GL/glew.h>
#include "Logger.h"

class Actor {
	protected:
		glm::quat       m_orientation;
		glm::vec4       m_position;
		glm::vec4       m_forward;
		glm::vec4       m_up;
		glm::vec4       m_left;
		
		RenderModel*    m_renderModel;
		PhysicsModel*   m_physicsModel;
	
	public:
		Actor();
		virtual ~Actor();
		
		virtual void translate(float longitude, float latitude, float altitude);
		virtual void rotate(float roll, float pitch, float yaw);
		
		virtual void update();
		virtual void draw(float* viewMatrix, float* projectionMatrix);
};

#endif
