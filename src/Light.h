#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct PointLight {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	float radius;

	PointLight(glm::vec3 pos, 
	           glm::vec3 clr, 
	           float its, 
	           float rad);
	
	virtual ~PointLight();
};

struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	float angle;
	
	SpotLight(glm::vec3 position, 
	          glm::vec3 direction, 
	          glm::vec3 color, 
	          float intensity, 
	          float angle);
	
	virtual ~SpotLight();
};

struct DirectionLight {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
	
	DirectionLight(glm::vec3 direction, 
	               glm::vec3 color, 
	               float intensity);
	
	virtual ~DirectionLight();
};

#endif
