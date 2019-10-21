#ifndef PD_RENDERCOMPONENT_HPP
#define PD_RENDERCOMPONENT_HPP

#include <string>

// RenderComponent contains all the information necessary to load and render
struct RenderComponent {
	std::string diffuse;
	std::string specular;
	std::string shader;
	std::string geometry;
};

#endif
