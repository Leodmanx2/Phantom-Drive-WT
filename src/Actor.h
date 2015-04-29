#ifndef ACTOR_H
#define ACTOR_H

#include <glm/glm.hpp>

class Actor {
	protected:
		glm::vec3*      m_vertices;
		unsigned int*   m_indices;
		glm::vec3*      m_normals;
		
		unsigned int    m_vertexCount;
		unsigned int    m_indexCount;
		unsigned int    m_normalCount;
		
		glm::vec3       m_position;
		glm::vec3       m_orientation;
		glm::vec3       m_forward;
		glm::vec3       m_up;
		glm::vec3       m_left;
		
		// GPU Resources
		unsigned int    m_vertexBuffer;
		unsigned int    m_indexBuffer;
		
		unsigned int    m_vertexShader;
		unsigned int    m_pixelShader;
		unsigned int    m_shaderProgram;
	
	public:
		Actor();
		virtual ~Actor();
		
		virtual void translate(float longitude, float latitude, float altitude);
		virtual void rotate(float roll, float pitch, float yaw);
		
		virtual void update();
		virtual void draw();
};

#endif
