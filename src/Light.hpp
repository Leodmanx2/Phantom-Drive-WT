#ifndef PD_LIGHT_HPP
#define PD_LIGHT_HPP

#include <glm/glm.hpp>

// Light can represent point lights or spot lights by changing the angle to be
// greater than or equal to or less than 2*pi, respectively. Direction lights
// can be approximated by setting the position to a minimum or maximum and the
// radius to max. (Rather, a direction light is an approximation of a very
// far off point light.)
struct Light final {
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
	float     intensity;
	float     angle;
	float     radius;

	Light(const glm::vec3& pos,
	      const glm::vec3& dir,
	      const glm::vec3& clr,
	      float            its,
	      float            agl,
	      float            rad);
};

#endif
