#ifndef ACTOR_H
#define ACTOR_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Actor {
	protected:
		glm::vec3       m_position;
		glm::vec3       m_orientation;
		glm::vec3       m_forward;
		glm::vec3       m_up;
		glm::vec3       m_left;
		
		// GPU Resources
		unsigned int    m_vertexBuffer;
	
	public:
		Actor();
		virtual ~Actor();
		
		virtual void translate(float longitude, float latitude, float altitude);
		virtual void rotate(float roll, float pitch, float yaw);
		
		virtual void update();
		virtual void draw();
};

#endif
