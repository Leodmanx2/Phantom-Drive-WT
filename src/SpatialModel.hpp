#ifndef PD_SPATIALMODEL_HPP
#define PD_SPATIALMODEL_HPP

#include <SWI-cpp.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

class SpatialModel final {
	private:
	glm::quat m_orientation;
	glm::vec4 m_position;

	public:
	static const glm::vec4 canonicalForward;
	static const glm::vec4 canonicalUp;
	static const glm::vec4 canonicalLeft;

	SpatialModel();

	void translate(float longitude, float latitude, float altitude);
	void rotate(float roll, float pitch, float yaw);

	void setOrientation(float deg, float x, float y, float z);
	void setPosition(float x, float y, float z);

	const glm::vec4& position() const;
	const glm::quat& orientation() const;
	const glm::mat4  matrix() const;
};

#endif
