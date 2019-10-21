#ifndef PD_LIGHT_HPP
#define PD_LIGHT_HPP

#include <glm/glm.hpp>

struct PointLight final {
	glm::vec3 position;
	glm::vec3 color;
	float     intensity;
	float     radius;

	PointLight(const glm::vec3& pos, const glm::vec3& clr, float its, float rad);
};

struct SpotLight final {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float     intensity;
	float     angle;
	float     radius;

	SpotLight(const glm::vec3& pos,
	          const glm::vec3& dir,
	          const glm::vec3& clr,
	          float            its,
	          float            agl,
	          float            rad);
};

struct DirectionLight final {
	glm::vec3 direction;
	glm::vec3 color;
	float     intensity;

	DirectionLight(const glm::vec3& dir, const glm::vec3& clr, float its);
};

#endif
