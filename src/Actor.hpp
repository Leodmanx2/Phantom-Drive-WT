#ifndef ACTOR_H
#define ACTOR_H

#include "Logger.hpp"
#include "RenderModel.hpp"
#include "Shader.hpp"
#include <GL/glew.h>
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class Actor {
	protected:
	Actor();
	glm::quat m_orientation;
	glm::vec4 m_position;
	glm::vec4 m_forward;
	glm::vec4 m_up;
	glm::vec4 m_left;

	std::shared_ptr<RenderModel> m_renderModel;

	public:
	Actor(std::shared_ptr<RenderModel> model);
	Actor(const Actor& original);
	virtual ~Actor();

	virtual void translate(float longitude, float latitude, float altitude);
	virtual void rotate(float roll, float pitch, float yaw);

	virtual void setOrientation(float deg, float x, float y, float z);
	virtual void setPosition(float x, float y, float z);

	virtual void draw(Shader& shader);
};

#endif
